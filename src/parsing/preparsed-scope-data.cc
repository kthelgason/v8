// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/parsing/preparsed-scope-data.h"

namespace v8 {
namespace internal {

PreParsedScopeData::ScopeScope::ScopeScope(PreParsedScopeData* data,
                                           ScopeType scope_type,
                                           int start_position, int end_position)
    : data_(data),
      previous_scope_(data->current_scope_),
      inner_scope_count_(0),
      variable_count_(0) {
  data->current_scope_ = this;
  if (previous_scope_ != nullptr) {
    ++previous_scope_->inner_scope_count_;
  }

  data->backing_store_.push_back(scope_type);
  data->backing_store_.push_back(start_position);
  data->backing_store_.push_back(end_position);
  // Reserve space for variable and inner scope count (we don't know yet how
  // many will be added).
  index_in_data_ = data->backing_store_.size();
  data->backing_store_.push_back(-1);
  data->backing_store_.push_back(-1);
}

PreParsedScopeData::ScopeScope::~ScopeScope() {
  data_->current_scope_ = previous_scope_;
  data_->backing_store_[index_in_data_] = inner_scope_count_;
  data_->backing_store_[index_in_data_ + 1] = variable_count_;
}

}  // namespace internal
}  // namespace v8
