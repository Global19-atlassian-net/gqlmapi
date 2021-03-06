// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "MAPIObjects.h"

#include "graphqlservice/introspection/Introspection.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using namespace std::literals;

namespace graphql::mapi {
namespace object {

Subscription::Subscription()
	: service::Object({
		"Subscription"
	}, {
		{ R"gql(items)gql"sv, [this](service::ResolverParams&& params) { return resolveItems(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } },
		{ R"gql(subFolders)gql"sv, [this](service::ResolverParams&& params) { return resolveSubFolders(std::move(params)); } },
		{ R"gql(rootFolders)gql"sv, [this](service::ResolverParams&& params) { return resolveRootFolders(std::move(params)); } }
	})
{
}

std::future<service::ResolverResult> Subscription::resolveItems(service::ResolverParams&& params)
{
	auto argFolderId = service::ModifiedArgument<mapi::ObjectId>::require("folderId", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getItems(service::FieldParams(std::move(params), std::move(directives)), std::move(argFolderId));
	resolverLock.unlock();

	return service::ModifiedResult<service::Object>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Subscription::resolveSubFolders(service::ResolverParams&& params)
{
	auto argParentFolderId = service::ModifiedArgument<mapi::ObjectId>::require("parentFolderId", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getSubFolders(service::FieldParams(std::move(params), std::move(directives)), std::move(argParentFolderId));
	resolverLock.unlock();

	return service::ModifiedResult<service::Object>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Subscription::resolveRootFolders(service::ResolverParams&& params)
{
	auto argStoreId = service::ModifiedArgument<response::IdType>::require("storeId", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getRootFolders(service::FieldParams(std::move(params), std::move(directives)), std::move(argStoreId));
	resolverLock.unlock();

	return service::ModifiedResult<service::Object>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Subscription::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(Subscription)gql" }, std::move(params));
}

} /* namespace object */

void AddSubscriptionDetails(std::shared_ptr<schema::ObjectType> typeSubscription, const std::shared_ptr<schema::Schema>& schema)
{
	typeSubscription->AddFields({
		schema::Field::Make(R"gql(items)gql"sv, R"md(Get updates on items in a folder.)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ItemChange")))), {
			schema::InputValue::Make(R"gql(folderId)gql"sv, R"md(ID of the folder)md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ObjectId")), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(subFolders)gql"sv, R"md(Get updates on sub-folders of a folder.)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("FolderChange")))), {
			schema::InputValue::Make(R"gql(parentFolderId)gql"sv, R"md(ID of the parent folder)md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ObjectId")), R"gql()gql"sv)
		}),
		schema::Field::Make(R"gql(rootFolders)gql"sv, R"md(Get updates on the root folders of a store.)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("FolderChange")))), {
			schema::InputValue::Make(R"gql(storeId)gql"sv, R"md(ID of the store)md"sv, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID")), R"gql()gql"sv)
		})
	});
}

} /* namespace graphql::mapi */
