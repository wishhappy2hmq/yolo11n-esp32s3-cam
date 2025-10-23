#pragma once
#include "dl_base_pad.hpp"
#include "dl_module_base.hpp"

namespace dl {
namespace module {

// https://onnx.ai/onnx/operators/onnx__Pad.html
class Pad : public Module {
private:
    std::vector<int> m_pads;
    padding_mode_t m_mode;
    TensorBase *m_constant_value;

public:
    /**
     * @brief Construct a new Pad object.
     *
     * @param name            name of module
     * @param inplace         inplace type.
     */
    Pad(std::vector<int> pads,
        padding_mode_t mode = PADDING_CONSTANT,
        TensorBase *constant_value = nullptr,
        const char *name = NULL,
        module_inplace_t inplace = MODULE_NON_INPLACE,
        quant_type_t quant_type = QUANT_TYPE_NONE) :
        Module(name, inplace, quant_type), m_pads(pads), m_mode(mode), m_constant_value(constant_value)
    {
    }

    /**
     * @brief Destroy the Pad object.
     */
    ~Pad() { delete m_constant_value; }

    std::vector<std::vector<int>> get_output_shape(std::vector<std::vector<int>> &input_shapes)
    {
        std::vector<int> output_shape = base::get_pad_shape(input_shapes[0], m_pads);

        return std::vector<std::vector<int>>(1, output_shape);
    }

    void forward(ModelContext *context, runtime_mode_t mode)
    {
        TensorBase *input = context->get_tensor(m_inputs_index[0]);
        TensorBase *output = context->get_tensor(m_outputs_index[0]);

        output->pad(input, m_pads, m_mode, m_constant_value);
    }

    void forward_args(void *args) {}

    /**
     * @brief deserialize Pad module instance by node serialization information
     */
    static Module *deserialize(fbs::FbsModel *fbs_model, std::string node_name)
    {
        Module *op = nullptr;
        quant_type_t quant_type;
        std::string mode;
        fbs_model->get_operation_attribute(node_name, "quant_type", quant_type);
        fbs_model->get_operation_attribute(node_name, "mode", mode);

        padding_mode_t padding_mode = PADDING_CONSTANT;
        if (mode == "constant") {
            padding_mode = PADDING_CONSTANT;
        } else if (mode == "edge") {
            padding_mode = PADDING_EDGE;
        } else if (mode == "reflect") {
            padding_mode = PADDING_REFLECT;
        } else {
            ESP_LOGE("Pad", "mode is not supported: %s", mode.c_str());
            assert(false);
        }

        TensorBase *pads = fbs_model->get_operation_parameter(node_name, 1);
        TensorBase *constant_value = fbs_model->get_operation_parameter(node_name, 2);
        std::vector<int> pads_index(pads->get_size(), 0);

        if (pads->get_dtype() == DATA_TYPE_INT64) {
            for (int i = 0; i < pads->get_size(); i++) {
                pads_index[i] = static_cast<int>(pads->get_element<int64_t>(i));
            }
        } else {
            ESP_LOGE("Pad", "pads only support int64");
        }

        // Create module
        op = new Pad(pads_index, padding_mode, constant_value, node_name.c_str(), MODULE_NON_INPLACE, quant_type);

        delete pads;
        return op;
    }

    void print()
    {
        ESP_LOGI("Pad", "quant_type: %s, pads: %s", quant_type_to_string(quant_type), vector_to_string(m_pads).c_str());
    }
};
} // namespace module
} // namespace dl
