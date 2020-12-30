/*
 * StreamingInterface.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2021 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FDBSERVER_STREAMINGINTERFACE_H
#define FDBSERVER_STREAMINGINTERFACE_H

#include "fdbclient/FDBTypes.h"
#include "fdbrpc/fdbrpc.h"
#include "fdbrpc/Locality.h"

// The interface for backup workers.
struct StreamingInterface {
	RequestStream<ReplyPromise<Void>> waitFailure;
	struct LocalityData locality;

	StreamingInterface() = default;
	explicit StreamingInterface(const struct LocalityData& l) : locality(l) {}

	void initEndpoints() {}
	UID id() const { return getToken(); }
	NetworkAddress address() const { return waitFailure.getEndpoint().getPrimaryAddress(); }
	UID getToken() const { return waitFailure.getEndpoint().token; }
	bool operator== (const StreamingInterface& r) const {
		return getToken() == r.getToken();
	}
	bool operator!= (const StreamingInterface& r) const {
		return !(*this == r);
	}

	template <class Archive>
	void serialize(Archive& ar) {
		serializer(ar, waitFailure, locality);
	}
};

#endif //FDBSERVER_STREAMINGINTERFACE_H