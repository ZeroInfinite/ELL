////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     ModelBuilderInterface.h (interfaces)
//  Authors:  Chuck Jacobs, Kirk Olynyk
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef SWIG

// emitters
#include "EmitterTypes.h"

// math
#include "Tensor.h"

// model
#include "Model.h"
#include "ModelBuilder.h"
#include "ModelInterface.h"
#include "NeuralNetworkPredictorInterface.h"

// stl
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#endif

namespace ELL_API
{

//
// Operation types for operation nodes
//
enum class UnaryOperationType
{
    none = (int)ell::emitters::UnaryOperationType::none,
    exp = (int)ell::emitters::UnaryOperationType::exp,
    log = (int)ell::emitters::UnaryOperationType::log,
    sqrt = (int)ell::emitters::UnaryOperationType::sqrt,
    logicalNot = (int)ell::emitters::UnaryOperationType::logicalNot,
    tanh = (int)ell::emitters::UnaryOperationType::tanh,
    square = (int)ell::emitters::UnaryOperationType::square,
    sin = (int)ell::emitters::UnaryOperationType::sin,
    cos = (int)ell::emitters::UnaryOperationType::cos
};

enum class BinaryOperationType
{
    none = (int)ell::emitters::BinaryOperationType::none,
    add = (int)ell::emitters::BinaryOperationType::add,
    subtract = (int)ell::emitters::BinaryOperationType::subtract,
    coordinatewiseMultiply = (int)ell::emitters::BinaryOperationType::coordinatewiseMultiply,
    coordinatewiseDivide = (int)ell::emitters::BinaryOperationType::coordinatewiseDivide,
    logicalAnd = (int)ell::emitters::BinaryOperationType::logicalAnd,
    logicalOr = (int)ell::emitters::BinaryOperationType::logicalOr,
    logicalXor = (int)ell::emitters::BinaryOperationType::logicalXor,
    shiftLeft = (int)ell::emitters::BinaryOperationType::shiftLeft,
    logicalShiftRight = (int)ell::emitters::BinaryOperationType::logicalShiftRight,
    arithmeticShiftRight = (int)ell::emitters::BinaryOperationType::arithmeticShiftRight
};

//
// ModelBuilder
//
class ModelBuilder
{
public:
    ModelBuilder();

    // Generic AddNode method
    Node AddNode(Model model, const std::string& nodeType, const std::vector<std::string>& args);

    // Specific methods per node type
    Node AddBinaryOperationNode(Model model, PortElements input1, PortElements input2, BinaryOperationType operation);
    Node AddBinaryOperationNodeWithMemoryLayout(Model model, PortElements input1, PortMemoryLayout input1Layout, PortElements input2, PortMemoryLayout input2Layout, PortMemoryLayout outputLayout, BinaryOperationType operation);
    Node AddBufferNode(Model model, PortElements input, int windowSize);
    Node AddTypeCastNode(Model model, PortElements input, PortType outputType);
    Node AddClockNode(Model model, PortElements input, double interval, double lagThreshold, const std::string& lagNotificationName);
    Node AddConcatenationNode(Model model, const ell::api::math::TensorShape& outputShape, const std::vector<PortElements*>& inputs);
    Node AddConstantNode(Model model, std::vector<double> values, PortType type);
    Node AddConstantNode(Model model, std::vector<double> values, const ell::api::math::TensorShape& outputShape, PortType type);
    Node AddDCTNode(Model model, PortElements input, int numFilters);
    Node AddNeuralNetworkPredictorNode(Model model, PortElements input, ell::api::predictors::NeuralNetworkPredictor predictor);
    Node AddFFTNode(Model model, PortElements input);
    Node AddHammingWindowNode(Model model, PortElements input);
    Node AddIIRFilterNode(Model model, PortElements input, std::vector<double> bCoeffs, std::vector<double> aCoeffs);
    InputNode AddInputNode(Model model, const ell::api::math::TensorShape& shape, PortType type);
    Node AddLinearFilterBankNode(Model model, PortElements input, double sampleRate, int numFilters, int numFiltersToUse);
    Node AddMelFilterBankNode(Model model, PortElements input, double sampleRate, int numFilters, int numFiltersToUse);
    OutputNode AddOutputNode(Model model, const ell::api::math::TensorShape& shape, PortElements input);
    Node AddReorderDataNode(Model model, PortElements input, PortMemoryLayout inputMemoryLayout, PortMemoryLayout outputMemoryLayout, std::vector<int> order, double outputPaddingValue = 0.0);
    Node AddSinkNode(Model model, PortElements input, const ell::api::math::TensorShape& shape, const std::string& sinkFunctionName, PortElements trigger = PortElements());
    Node AddSourceNode(Model model, PortElements input, PortType outputType, const ell::api::math::TensorShape& shape, const std::string& sourceFunctionName);
    Node AddUnaryOperationNode(Model model, PortElements input, UnaryOperationType operation);
    Node AddDTWNode(Model model, std::vector<std::vector<double>> prototype, PortElements input);
    Node AddVoiceActivityDetectorNode(Model model, PortElements input, double sampleRate, double frameDuration,
        double tauUp,  double tauDown, double largeInput, double gainAtt, double thresholdUp, double thresholdDown,
        double levelThreshold);
    Node AddRNNNode(Model model, PortElements input, PortElements reset, size_t hiddenUnits,
        PortElements inputWeights, PortElements hiddenWeights, PortElements inputBias, PortElements hiddenBias,
        ell::api::predictors::neural::ActivationType activation);
    Node AddGRUNode(Model model, PortElements input, PortElements reset, size_t hiddenUnits, 
        PortElements inputWeights, PortElements hiddenWeights, PortElements inputBias, PortElements hiddenBias,
        ell::api::predictors::neural::ActivationType activation, ell::api::predictors::neural::ActivationType recurrentActivation);
    Node AddLSTMNode(Model model, PortElements input, PortElements reset, size_t hiddenUnits, 
        PortElements inputWeights, PortElements hiddenWeights, PortElements inputBias, PortElements hiddenBias,
        ell::api::predictors::neural::ActivationType activation, ell::api::predictors::neural::ActivationType recurrentActivation);

    // Layer nodes (going away...)
    Node AddActivationLayerNode(Model model, PortElements input, const ell::api::predictors::neural::ActivationLayer& layer);
    Node AddBatchNormalizationLayerNode(Model model, PortElements input, const ell::api::predictors::neural::BatchNormalizationLayer& layer);
    Node AddBiasLayerNode(Model model, PortElements input, const ell::api::predictors::neural::BiasLayer& layer);
    Node AddBinaryConvolutionalLayerNode(Model model, PortElements input, const ell::api::predictors::neural::BinaryConvolutionalLayer& layer);
    Node AddConvolutionalLayerNode(Model model, PortElements input, const ell::api::predictors::neural::ConvolutionalLayer& layer);
    Node AddFullyConnectedLayerNode(Model model, PortElements input, const ell::api::predictors::neural::FullyConnectedLayer& layer);
    Node AddRegionDetectionLayerNode(Model model, PortElements input, const ell::api::predictors::neural::RegionDetectionLayer& layer);
    Node AddPoolingLayerNode(Model model, PortElements input, const ell::api::predictors::neural::PoolingLayer& layer);
    Node AddScalingLayerNode(Model model, PortElements input, const ell::api::predictors::neural::ScalingLayer& layer);
    Node AddSoftmaxLayerNode(Model model, PortElements input, const ell::api::predictors::neural::SoftmaxLayer& layer);

    void ResetInput(Node node, PortElements input, std::string input_port_name = "input");
private:
#ifndef SWIG
    template <typename ElementType>
    Node AddNeuralNetworkPredictorNode(Model model, PortElements input, const ell::predictors::NeuralNetworkPredictor<ElementType>& predictor);

    template <typename ElementType>
    Node AddActivationLayerNode(Model model, PortElements input, const ell::api::predictors::neural::ActivationLayer& layer);

    template <typename ElementType>
    Node AddBatchNormalizationLayerNode(Model model, PortElements input, const ell::api::predictors::neural::BatchNormalizationLayer& layer);

    template <typename ElementType>
    Node AddBiasLayerNode(Model model, PortElements input, const ell::api::predictors::neural::BiasLayer& layer);

    template <typename ElementType>
    Node AddBinaryConvolutionalLayerNode(Model model, PortElements input, const ell::api::predictors::neural::BinaryConvolutionalLayer& layer);

    template <typename ElementType>
    Node AddConvolutionalLayerNode(Model model, PortElements input, const ell::api::predictors::neural::ConvolutionalLayer& layer);

    template <typename ElementType>
    Node AddFullyConnectedLayerNode(Model model, PortElements input, const ell::api::predictors::neural::FullyConnectedLayer& layer);

    template <typename ElementType>
    Node AddRegionDetectionLayerNode(Model model, PortElements input, const ell::api::predictors::neural::RegionDetectionLayer& layer);

    template <typename ElementType>
    Node AddPoolingLayerNode(Model model, PortElements input, const ell::api::predictors::neural::PoolingLayer& layer);

    template <typename ElementType>
    Node AddScalingLayerNode(Model model, PortElements input, const ell::api::predictors::neural::ScalingLayer& layer);

    template <typename ElementType>
    Node AddSoftmaxLayerNode(Model model, PortElements input, const ell::api::predictors::neural::SoftmaxLayer& layer);

    template <typename ElementType>
    Node AddRNNNode(Model model, PortElements input, PortElements reset, size_t hiddenUnits, PortElements inputWeights, PortElements hiddenWeights, PortElements inputBias, PortElements hiddenBias,
        ell::api::predictors::neural::ActivationType activation);

    template <typename ElementType>
    Node AddLSTMNode(Model model, PortElements input, PortElements reset, size_t hiddenUnits, PortElements inputWeights, PortElements hiddenWeights, PortElements inputBias, PortElements hiddenBias,
        ell::api::predictors::neural::ActivationType activation, ell::api::predictors::neural::ActivationType recurrentActivation);

    template <typename ElementType>
    Node AddGRUNode(Model model, PortElements input, PortElements reset, size_t hiddenUnits, PortElements inputWeights, PortElements hiddenWeights, PortElements inputBias, PortElements hiddenBias,
        ell::api::predictors::neural::ActivationType activation, ell::api::predictors::neural::ActivationType recurrentActivation);

    template <typename ElementType>
    void InternalResetInput(Node node, PortElements input, std::string input_port_name);
#endif

    ell::model::ModelBuilder _modelBuilder;
};


#ifndef SWIG
template <typename OutputType, typename InputType>
std::vector<OutputType> CastVector(const std::vector<InputType>& vector);
#endif

} // end namespace

