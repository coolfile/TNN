// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include "abstract_layer_interpreter.h"

namespace TNN_NS {
DECLARE_LAYER_INTERPRETER(ScatterND, LAYER_SCATTER_ND);

Status ScatterNDLayerInterpreter::InterpretProto(str_arr layer_cfg_arr, int start_index, LayerParam **param) {
    return TNN_OK;
}

Status ScatterNDLayerInterpreter::InterpretResource(Deserializer &deserializer, LayerResource **resource) {
    auto layer_resource = CreateLayerRes<ScatterNDLayerResource>(resource);
    auto layer_name     = deserializer.GetString();
    bool has_indices    = deserializer.GetBool();
    if (has_indices) {
        deserializer.GetDims(layer_resource->indices_dims);
        GET_BUFFER_FOR_ATTR(layer_resource, indices, deserializer);
    }
    bool has_update = deserializer.GetBool();
    if (has_update) {
        deserializer.GetDims(layer_resource->updates_dims);
        GET_BUFFER_FOR_ATTR(layer_resource, updates, deserializer);
    }
    return TNN_OK;
}

Status ScatterNDLayerInterpreter::SaveProto(std::ofstream &output_stream, LayerParam *param) {
    return TNN_OK;
}

Status ScatterNDLayerInterpreter::SaveResource(Serializer &serializer, LayerParam *param, LayerResource *resource) {
    CAST_OR_RET_ERROR(layer_param, LayerParam, "invalid layer param", param);
    CAST_OR_RET_ERROR(layer_resource, ScatterNDLayerResource, "invalid layer res to save", resource);
    serializer.PutString(layer_param->name);
    bool has_indices = !layer_resource->indices_dims.empty();
    if (has_indices) {
        serializer.PutBool(true);
        serializer.PutDims(layer_resource->indices_dims);
        serializer.PutRaw(layer_resource->indices);
    } else {
        serializer.PutBool(true);
    }
    bool has_update = !layer_resource->updates_dims.empty();
    if (has_update) {
        serializer.PutBool(true);
        serializer.PutDims(layer_resource->updates_dims);
        serializer.PutRaw(layer_resource->updates);
    } else {
        serializer.PutInt(false);
    }
    return TNN_OK;
}

REGISTER_LAYER_INTERPRETER(ScatterND, LAYER_SCATTER_ND);

}  // namespace TNN_NS