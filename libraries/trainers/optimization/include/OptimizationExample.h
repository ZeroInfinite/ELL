////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Learning Library (ELL)
//  File:     Example.h (optimization)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//stl
#include <memory>

namespace ell
{
namespace trainers
{
namespace optimization
{
    /// <summary> Represents an example for an Empirical Risk Minimization optimizer</summary>
    template <typename InputT, typename OutputT>
    struct Example
    {
        using InputType = InputT;
        using OutputType = OutputT;

        Example() = default;
        Example(const Example& other) = default;
        Example(Example&&) = default;

        /// <summary> Constructor. </summary>
        Example(InputType input, OutputType output, double weight = 1.0);

        /// <summary> Upcasting copy constructor. </summary>
        template <typename InputBase, typename OutputBase>
        Example(const Example<InputBase, OutputBase>& other);

        /// <summary> The input or instance. </summary>
        InputType input;

        /// <summary> The output or label. </summary>
        OutputType output;

        /// <summary> The example weight. </summary>
        double weight;
    };
}
}
}

#include "../tcc/OptimizationExample.tcc"