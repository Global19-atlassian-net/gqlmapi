// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef FOLDEROBJECT_H
#define FOLDEROBJECT_H

#include "MAPISchema.h"

namespace graphql::mapi::object {

class Folder
	: public service::Object
{
protected:
	explicit Folder();

public:
	virtual service::FieldResult<response::IdType> getId(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::shared_ptr<Folder>> getParentFolder(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::shared_ptr<Store>> getStore(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<response::StringType> getName(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<response::IntType> getCount(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<response::IntType> getUnread(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::optional<SpecialFolder>> getSpecialFolder(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::vector<std::shared_ptr<Property>>> getColumns(service::FieldParams&& params) const = 0;
	virtual service::FieldResult<std::vector<std::shared_ptr<Folder>>> getSubFolders(service::FieldParams&& params, std::optional<std::vector<response::IdType>>&& idsArg) const = 0;
	virtual service::FieldResult<std::vector<std::shared_ptr<Conversation>>> getConversations(service::FieldParams&& params, std::optional<std::vector<response::IdType>>&& idsArg) const = 0;
	virtual service::FieldResult<std::vector<std::shared_ptr<Item>>> getItems(service::FieldParams&& params, std::optional<std::vector<response::IdType>>&& idsArg) const = 0;

private:
	std::future<service::ResolverResult> resolveId(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveParentFolder(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveStore(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveName(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveCount(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveUnread(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveSpecialFolder(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveColumns(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveSubFolders(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveConversations(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolveItems(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
};

} /* namespace graphql::mapi::object */

#endif // FOLDEROBJECT_H
