// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef FILEATTACHMENTOBJECT_H
#define FILEATTACHMENTOBJECT_H

#include "MAPISchema.h"

namespace graphql::mapi::object {

class FileAttachment
	: public service::Object
{
protected:
	explicit FileAttachment();

public:
	virtual service::FieldResult<response::IdType> getId(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::optional<response::Value>> getContents(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::vector<std::shared_ptr<Property>>> getProperties(service::FieldParams&& params, std::optional<std::vector<PropIdInput>>&& idsArg) const = 0;

private:
	std::future<service::ResolverResult> resolveId(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveName(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveContents(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveProperties(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

} /* namespace graphql::mapi::object */

#endif // FILEATTACHMENTOBJECT_H
