////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     MatrixVectorProductNode.tcc (nodes)
//  Authors:  Suresh Iyengar
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ConstantNode.h"
#include "MatrixVectorMultiplyNode.h"

// math
#include "MatrixOperations.h"

// utilities
#include "Exception.h"

// stl
#include <vector>

namespace ell
{
namespace nodes
{
    template <typename ValueType, math::MatrixLayout layout>
    MatrixVectorProductNode<ValueType, layout>::MatrixVectorProductNode()
        : Node({ &_input }, { &_output }), _input(this, {}, defaultInputPortName), _output(this, defaultOutputPortName, 1), _w(0, 0)
    {
    }

    template <typename ValueType, math::MatrixLayout layout>
    MatrixVectorProductNode<ValueType, layout>::MatrixVectorProductNode(const model::OutputPort<ValueType>& input, const math::Matrix<ValueType, layout>& w)
        : Node({ &_input }, { &_output }), _input(this, input, defaultInputPortName), _output(this, defaultOutputPortName, w.NumRows()), _w(w)
    {
        if (input.Size() != w.NumColumns())
        {
            throw utilities::InputException(utilities::InputExceptionErrors::invalidArgument, "MatrixVectorProductNode: input size must match the number of columns in the 'w' matrix");
        }
    }

    template <typename ValueType, math::MatrixLayout layout>
    void MatrixVectorProductNode<ValueType, layout>::WriteToArchive(utilities::Archiver& archiver) const
    {
        Node::WriteToArchive(archiver);

        archiver["w_rows"] << _w.NumRows();
        archiver["w_columns"] << _w.NumColumns();
        std::vector<ValueType> temp;
        temp.assign(_w.GetConstDataPointer(), _w.GetConstDataPointer() + (size_t)(_w.NumRows() * _w.NumColumns()));
        archiver["w"] << temp;

        archiver[defaultInputPortName] << _input;
        archiver[defaultOutputPortName] << _output;
    }

    template <typename ValueType, math::MatrixLayout layout>
    void MatrixVectorProductNode<ValueType, layout>::ReadFromArchive(utilities::Unarchiver& archiver)
    {
        Node::ReadFromArchive(archiver);

        size_t w_rows = 0;
        size_t w_columns = 0;
        archiver["w_rows"] >> w_rows;
        archiver["w_columns"] >> w_columns;
        std::vector<ValueType> temp;
        archiver["w"] >> temp;
        _w = math::Matrix<ValueType, layout>(w_rows, w_columns, temp);

        archiver[defaultInputPortName] >> _input;
        archiver[defaultOutputPortName] >> _output;
    }

    template <typename ValueType, math::MatrixLayout layout>
    void MatrixVectorProductNode<ValueType, layout>::Copy(model::ModelTransformer& transformer) const
    {
        const auto& newPortElements = transformer.GetCorrespondingInputs(_input);
        auto newNode = transformer.AddNode<MatrixVectorProductNode<ValueType, layout>>(newPortElements, _w);
        transformer.MapNodeOutput(output, newNode->output);
    }

    template <typename ValueType, math::MatrixLayout layout>
    bool MatrixVectorProductNode<ValueType, layout>::Refine(model::ModelTransformer& transformer) const
    {
        const auto& newInput = transformer.GetCorrespondingInputs(_input);

        // Make sure we have a RowMatrix (because that's what MatrixVectorMultiplyNode wants)
        math::RowMatrix<ValueType> projectionMatrix(_w);
        auto projectionMatrixValue = projectionMatrix.ToArray();
        auto projectionMatrixNode = transformer.AddNode<ConstantNode<ValueType>>(projectionMatrixValue);
        auto m = projectionMatrix.NumRows();
        auto n = projectionMatrix.NumColumns();
        auto matrixStride = projectionMatrix.GetIncrement();
        if(matrixStride == 0 || matrixStride < m)
        {
            utilities::InputException(utilities::InputExceptionErrors::badData, "Matrix has an invalid stride");
        }
        auto matrixMultiplyNode = transformer.AddNode<MatrixVectorMultiplyNode<ValueType>>(projectionMatrixNode->output, m, n, matrixStride, newInput);
        transformer.MapNodeOutput(output, matrixMultiplyNode->output);
        return true;
    }

    template <typename ValueType, math::MatrixLayout layout>
    void MatrixVectorProductNode<ValueType, layout>::Compute() const
    {
        math::ColumnVector<ValueType> input(_input.Size());
        for (size_t index = 0; index < _input.Size(); ++index)
        {
            input[index] = _input[index];
        }

        math::ColumnVector<ValueType> result(_w.NumRows());

        // result = _w * data
        math::MultiplyScaleAddUpdate(static_cast<ValueType>(1), _w, input, static_cast<ValueType>(0), result);

        _output.SetOutput(result.ToArray());
    }

    template <typename ValueType, math::MatrixLayout layout>
    MatrixVectorProductNode<ValueType, layout>* AddNodeToModelTransformer(const model::PortElements<ValueType>& input, math::ConstMatrixReference<ValueType, layout> w, model::ModelTransformer& transformer)
    {
        return transformer.AddNode<MatrixVectorProductNode>(input, w);
    }
}
}
