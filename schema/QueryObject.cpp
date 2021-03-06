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

Query::Query()
	: service::Object({
		"Query"
	}, {
		{ R"gql(__type)gql"sv, [this](service::ResolverParams&& params) { return resolve_type(std::move(params)); } },
		{ R"gql(stores)gql"sv, [this](service::ResolverParams&& params) { return resolveStores(std::move(params)); } },
		{ R"gql(__schema)gql"sv, [this](service::ResolverParams&& params) { return resolve_schema(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } }
	})
	, _schema(GetSchema())
{
}

std::future<service::ResolverResult> Query::resolveStores(service::ResolverParams&& params)
{
	auto argIds = service::ModifiedArgument<response::IdType>::require<service::TypeModifier::Nullable, service::TypeModifier::List>("ids", params.arguments);
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getStores(service::FieldParams(std::move(params), std::move(directives)), std::move(argIds));
	resolverLock.unlock();

	return service::ModifiedResult<Store>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> Query::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(Query)gql" }, std::move(params));
}

std::future<service::ResolverResult> Query::resolve_schema(service::ResolverParams&& params)
{
	return service::ModifiedResult<service::Object>::convert(std::static_pointer_cast<service::Object>(std::make_shared<introspection::Schema>(_schema)), std::move(params));
}

std::future<service::ResolverResult> Query::resolve_type(service::ResolverParams&& params)
{
	auto argName = service::ModifiedArgument<response::StringType>::require("name", params.arguments);
	const auto& baseType = _schema->LookupType(argName);
	std::shared_ptr<introspection::object::Type> result { baseType ? std::make_shared<introspection::Type>(baseType) : nullptr };

	return service::ModifiedResult<introspection::object::Type>::convert<service::TypeModifier::Nullable>(result, std::move(params));
}

} /* namespace object */

void AddQueryDetails(std::shared_ptr<schema::ObjectType> typeQuery, const std::shared_ptr<schema::Schema>& schema)
{
	typeQuery->AddFields({
		schema::Field::Make(R"gql(stores)gql"sv, R"md(List of stores, which may include stores that are not associated with any account.)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Store")))), {
			schema::InputValue::Make(R"gql(ids)gql"sv, R"md(Optional list of store IDs, return all stores if `null`)md"sv, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("ID"))), R"gql(null)gql"sv)
		})
	});
}

} /* namespace graphql::mapi */
