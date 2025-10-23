#pragma once

#include "dl_base_elemwise.hpp"
#include "dl_base_pow.hpp" // Include the new pow operation
#include "dl_base_shape.hpp"
#include "dl_math.hpp"
#include "dl_module_base.hpp"

namespace dl {
namespace module {

/**
 * @brief: Pow takes input data (Tensor) and exponent Tensor, and produces one output data (Tensor) where the function
 * f(x) = x^exponent, is applied to the data tensor elementwise. Supports float, int16_t and int8_t. This operator
 * supports multidirectional (i.e., Numpy-style) broadcasting.
 */
class Pow : public Module {
public:
    /**
     * @brief Construct a new Pow object.
     *
     * @param name            name of module
     * @param inplace         inplace type.
     */
    Pow(const char *name = NULL,
        module_inplace_t inplace = MODULE_NON_INPLACE,
        quant_type_t quant_type = QUANT_TYPE_NONE) :
        Module(name, inplace, quant_type)
    {
    }

    /**
     * @brief Destroy the Pow object.
     */
    ~Pow() {}

    std::vector<std::vector<int>> get_output_shape(std::vector<std::vector<int>> &input_shapes)
    {
        // support multidirectional broadcasting
        std::vector<int> output_shape = base::get_multidirectional_broadcasting_shape(input_shapes[0], input_shapes[1]);

        return std::vector<std::vector<int>>(1, output_shape);
    }

    void forward(ModelContext *context, runtime_mode_t mode = RUNTIME_MODE_AUTO)
    {
        if (quant_type == QUANT_TYPE_SYMM_8BIT) {
            forward_template<int8_t>(context, mode);
        } else if (quant_type == QUANT_TYPE_SYMM_16BIT) {
            forward_template<int16_t>(context, mode);
        } else if (quant_type == QUANT_TYPE_FLOAT32) {
            forward_template<float>(context, mode);
        }
    }

    void forward_args(void *args)
    {
        if (quant_type == QUANT_TYPE_SYMM_8BIT) {
            base::elemwise_pow((base::elemwiseArgsType<int8_t> *)args);
        } else if (quant_type == QUANT_TYPE_SYMM_16BIT) {
            base::elemwise_pow((base::elemwiseArgsType<int16_t> *)args);
        } else if (quant_type == QUANT_TYPE_FLOAT32) {
            base::elemwise_pow((base::elemwiseArgsType<float> *)args);
        }
    }

    template <typename T>
    void forward_template(ModelContext *context, runtime_mode_t mode)
    {
        TensorBase *input0 = context->get_tensor(m_inputs_index[0]);
        TensorBase *input1 = context->get_tensor(m_inputs_index[1]);
        TensorBase *output = context->get_tensor(m_outputs_index[0]);

        std::vector<base::elemwiseArgsType<T>> m_args =
            base::get_elemwise_operation_args<T>(output, input0, input1, mode);
        int task_size = m_args.size();
        if (task_size == 1) { // single task
            forward_args((void *)&m_args[0]);
        } else if (task_size == 2) { // multi task, use semaphore to maintain synchronization.
            module_forward_dual_core(this, (void *)&m_args[0], (void *)&m_args[1]);
        } else {
            ESP_LOGE("Pow", "Only support task size is 1 or 2, currently task size is %d", task_size);
        }
    }

    /**
     * @brief deserialize Pow module instance by node serialization information
     */
    static Module *deserialize(fbs::FbsModel *fbs_model, std::string node_name)
    {
        Module *op = nullptr;
        quant_type_t quant_type;
        fbs_model->get_operation_attribute(node_name, "quant_type", quant_type);

        // Create module
        if (quant_type == QUANT_TYPE_SYMM_8BIT || quant_type == QUANT_TYPE_SYMM_16BIT ||
            quant_type == QUANT_TYPE_FLOAT32) {
            op = new Pow(node_name.c_str(), MODULE_NON_INPLACE, quant_type);
        }
        return op;
    }

    void print()
    {
        ESP_LOGI("Pow",
                 "quant_type: %s, input feature map size: %d.",
                 quant_type_to_string(quant_type),
                 m_inputs_index.size());
    }
};

} // namespace module
} // namespace dl
