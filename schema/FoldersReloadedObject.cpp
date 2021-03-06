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

FoldersReloaded::FoldersReloaded()
	: service::Object({
		"FolderChange",
		"FoldersReloaded"
	}, {
		{ R"gql(reloaded)gql"sv, [this](service::ResolverParams&& params) { return resolveReloaded(std::move(params)); } },
		{ R"gql(__typename)gql"sv, [this](service::ResolverParams&& params) { return resolve_typename(std::move(params)); } }
	})
{
}

std::future<service::ResolverResult> FoldersReloaded::resolveReloaded(service::ResolverParams&& params)
{
	std::unique_lock resolverLock(_resolverMutex);
	auto directives = std::move(params.fieldDirectives);
	auto result = getReloaded(service::FieldParams(std::move(params), std::move(directives)));
	resolverLock.unlock();

	return service::ModifiedResult<Folder>::convert<service::TypeModifier::List>(std::move(result), std::move(params));
}

std::future<service::ResolverResult> FoldersReloaded::resolve_typename(service::ResolverParams&& params)
{
	return service::ModifiedResult<response::StringType>::convert(response::StringType{ R"gql(FoldersReloaded)gql" }, std::move(params));
}

} /* namespace object */

void AddFoldersReloadedDetails(std::shared_ptr<schema::ObjectType> typeFoldersReloaded, const std::shared_ptr<schema::Schema>& schema)
{
	typeFoldersReloaded->AddFields({
		schema::Field::Make(R"gql(reloaded)gql"sv, R"md(`Folders` that were reloaded)md"sv, std::nullopt, schema->WrapType(introspection::TypeKind::NON_NULL, schema->WrapType(introspection::TypeKind::LIST, schema->WrapType(introspection::TypeKind::NON_NULL, schema->LookupType("Folder")))))
	});
}

} /* namespace graphql::mapi */
