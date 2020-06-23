/**
 * @file libyang.h
 * @author Radek Krejci <rkrejci@cesnet.cz>
 * @brief The main libyang public header.
 *
 * Copyright (c) 2015 - 2018 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef LY_LIBYANG_H_
#define LY_LIBYANG_H_

#include <stdio.h>

/**
 * @brief Whether to enable schema and data cache.
 */
#define LY_ENABLED_CACHE

/**
 * @brief Whether to allow reusing latest revisions of schemas.
 */
#define LY_ENABLED_LATEST_REVISIONS

/**
 * @brief Whether to add priv pointer to struct lyd_node.
 */
/* #undef LY_ENABLED_LYD_PRIV */

/**
 * @brief Compiler flag for packed data types.
 */
#define _PACKED __attribute__((__packed__))

#include "tree_schema.h"
#include "tree_data.h"
#include "xml.h"
#include "dict.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LY_VERSION_MAJOR 1 /**< libyang major version number */
#define LY_VERSION_MINOR 8 /**< libyang minor version number */
#define LY_VERSION_MICRO 4 /**< libyang micro version number */
#define LY_VERSION "1.8.4" /**< libyang version string */

/**
 * @mainpage About
 *
 * libyang is a library implementing processing of the YANG schemas and data modeled by the YANG language. The
 * library is implemented in C for GNU/Linux and provides C API.
 *
 * @section about-features Main Features
 *
 * - [Parsing (and validating) schemas](@ref howtoschemasparsers) in YANG format.
 * - [Parsing (and validating) schemas](@ref howtoschemasparsers) in YIN format.
 * - [Parsing, validating and printing instance data](@ref howtodata) in XML format.
 * - [Parsing, validating and printing instance data](@ref howtodata) in JSON format
 *   ([RFC 7951](https://tools.ietf.org/html/rfc7951)).
 * - [Manipulation with the instance data](@ref howtodatamanipulators).
 * - Support for [default values in the instance data](@ref howtodatawd) ([RFC 6243](https://tools.ietf.org/html/rfc6243)).
 * - Support for [YANG extensions and user types](@ref howtoschemaplugins).
 * - Support for [YANG Metadata](@ref howtoschemametadata) ([RFC 7952](https://tools.ietf.org/html/rfc6243)).
 *
 * The current implementation covers YANG 1.0 ([RFC 6020](https://tools.ietf.org/html/rfc6020)) as well as
 * YANG 1.1 ([RFC 7950](https://tools.ietf.org/html/rfc7950)).
 *
 * @subsection about-features-others Extra (side-effect) Features
 *
 * - XML parser.
 * - Optimized string storage (dictionary).
 *
 * @section about-license License
 *
 * Copyright (c) 2015-2017 CESNET, z.s.p.o.
 *
 * (The BSD 3-Clause License)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Company nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 */

/**
 * @page howto How To ...
 *
 * - @subpage howtocontext
 * - @subpage howtoschemas
 * - @subpage howtodata
 * - @subpage howtoxpath
 * - @subpage howtoxml
 * - @subpage howtothreads
 * - @subpage howtologger
 */

/** @page howtocontext Context
 *
 * The context concept allows callers to work in environments with different sets of YANG schemas.
 *
 * The first step in libyang is to create a new context using ly_ctx_new(). It returns a handler
 * used in the following work.
 *
 * When creating a new context, search dir can be specified (NULL is accepted) to provide directory
 * where libyang will automatically search for schemas being imported or included. The search path
 * can be later changed via ly_ctx_set_searchdir() and ly_ctx_unset_searchdirs() functions. Before the search dirs,
 * also the current working directory is (non-recursively) searched. For the case of the explicitly set search
 * dirs, also all their subdirectories (and symlinks) are taken into account. Searching in the current working
 * directory can be avoided with context's #LY_CTX_DISABLE_SEARCHDIR_CWD option (or via ly_ctx_set_disable_searchdir_cwd()).
 * Searching in all the context's search dirs (without removing them) can be avoided with the context's
 * #LY_CTX_DISABLE_SEARCHDIRS option (or via ly_ctx_set_disable_searchdirs()). This automatic searching can be preceded
 * by a custom  module searching callback (#ly_module_imp_clb) set via ly_ctx_set_module_imp_clb(). The algorithm of
 * searching in search dirs is also available via API as lys_search_localfile() function.
 *
 * Schemas are added into the context using [parser functions](@ref howtoschemasparsers) - \b lys_parse_*().
 * In case of schemas, also ly_ctx_load_module() can be used - in that case the #ly_module_imp_clb or automatic
 * search in search dir and in the current working directory is used.
 *
 * Similarly, data trees can be parsed by \b lyd_parse_*() functions. Note, that functions for schemas have \b lys_
 * prefix while functions for instance data have \b lyd_ prefix. It can happen during data parsing that a schema is
 * required and __not found__ in the context or the schema is found, but is __only imported__, not implemented (so the
 * data cannot actually be instantiated). In these cases, a callback is called, which should add this schema into
 * the context or change its conformance to implemented. You can set the callback using ly_ctx_set_module_data_clb()
 * (more in @ref howtodataparsers and @ref howtodatavalidation).
 *
 * Context can hold multiple revisions of the same schema, but only one of them can be implemented. The schema is not
 * implemented in case it is automatically loaded as import for another module and it is not referenced in such
 * a module (and no other) as target of leafref, augment or deviation. All modules with deviation definition are always
 * marked as implemented. The imported (not implemented) module can be set implemented by lys_set_implemented(). But
 * the implemented module cannot be changed back to just imported module. The imported modules are used only as a
 * source of definitions for types and groupings for uses statements. The data in such modules are ignored - caller
 * is not allowed to create the data (including instantiating identities) defined in the model via data parsers,
 * the default nodes are not added into any data tree and mandatory nodes are not checked in the data trees. This
 * can be changed by ly_ctx_set_allimplemented() function, which causes that all the imported modules are automatically
 * set to be implemented.
 *
 * When loading/importing a module without revision, the latest revision of the required module is supposed to load.
 * For a context, the first time the latest revision of a module is requested, it is properly searched for and loaded.
 * However, when this module is requested (without revision) the second time, the one found previously is returned.
 * This has the advantage of not searching for the module repeatedly but the drawback that if a later revision
 * of the module is later made available, this context will not use it. If you are aware of a case when this
 * optimization could cause problems, you can disable it using a cmake(1) build option (variable).
 *
 * Context holds all modules and their submodules internally. To get
 * a specific module or submodule, use ly_ctx_get_module() and ly_ctx_get_submodule(). There are some additional
 * alternatives to these functions (with different parameters). If you need to do something with all the modules or
 * submodules in the context, it is advised to iterate over them using ly_ctx_get_module_iter(), it is
 * the most efficient way. Alternatively, the ly_ctx_info() function can be used to get complex information
 * about the schemas in the context in the form of data tree defined by
 * <a href="https://tools.ietf.org/html/draft-ietf-netconf-yang-library-04">ietf-yang-library</a> schema.
 * To get a specific node defined in a module in the context, ly_ctx_find_path() or ly_ctx_get_node() can be used.
 *
 * Modules held by a context can be removed by ly_ctx_remove_module(). Besides removing the module, it is possible
 * just to disable it with lys_set_disabled(). In this case the module is hidden in the context (disabled modules can
 * be iterated via ly_ctx_get_disabled_module_iter()) and not used during the common work with the context. The module
 * is enabled explicitly by calling lys_set_enabled() or implicitly by the request to load the schema (directly or
 * indirectly via import of another module) into the context.
 *
 * To clean the context from all the loaded modules (except the [internal modules](@ref howtoschemasparsers)), the
 * ly_ctx_clean() function can be used. To remove the context, there is ly_ctx_destroy() function.
 *
 * - @subpage howtocontextdict
 *
 * \note API for this group of functions is available in the [context module](@ref context).
 *
 * Functions List
 * --------------
 * - ly_ctx_new()
 * - ly_ctx_set_searchdir()
 * - ly_ctx_unset_searchdirs()
 * - ly_ctx_get_searchdirs()
 * - ly_ctx_set_module_imp_clb()
 * - ly_ctx_get_module_imp_clb()
 * - ly_ctx_set_module_data_clb()
 * - ly_ctx_get_module_data_clb()
 * - ly_ctx_set_allimplemented()
 * - ly_ctx_unset_allimplemented()
 * - ly_ctx_set_disable_searchdirs()
 * - ly_ctx_unset_disable_searchdirs()
 * - ly_ctx_set_disable_searchdir_cwd()
 * - ly_ctx_unset_disable_searchdir_cwd()
 * - ly_ctx_load_module()
 * - ly_ctx_info()
 * - ly_ctx_get_module_set_id()
 * - ly_ctx_get_module_iter()
 * - ly_ctx_get_disabled_module_iter()
 * - ly_ctx_get_module()
 * - ly_ctx_get_module_older()
 * - ly_ctx_get_module_by_ns()
 * - ly_ctx_get_submodule()
 * - ly_ctx_get_submodule2()
 * - ly_ctx_get_node()
 * - ly_ctx_find_path()
 * - ly_ctx_remove_module()
 * - ly_ctx_clean()
 * - ly_ctx_destroy()
 * - lys_set_implemented()
 * - lys_set_disabled()
 * - lys_set_enabled()
 * - lys_search_localfile()
 */

/**
 * @page howtocontextdict Context Dictionary
 *
 * Context includes dictionary to store strings more effectively. The most of strings repeats quite often in schema
 * as well as data trees. Therefore, instead of allocating those strings each time they appear, libyang stores them
 * as records in the dictionary. The basic API to the context dictionary is public, so even a caller application can
 * use the dictionary.
 *
 * To insert a string into the dictionary, caller can use lydict_insert() (adding a constant string) or
 * lydict_insert_zc() (for dynamically allocated strings that won't be used by the caller after its insertion into
 * the dictionary). Both functions return the pointer to the inserted string in the dictionary record.
 *
 * To remove (reference of the) string from the context dictionary, lydict_remove() is supposed to be used.
 *
 * \note Incorrect usage of the dictionary can break libyang functionality.
 *
 * \note API for this group of functions is described in the [XML Parser module](@ref dict).
 *
 * Functions List
 * --------------
 * - lydict_insert()
 * - lydict_insert_zc()
 * - lydict_remove()
 */

/**
 * @page howtoschemas Schemas
 *
 *
 * Schema is an internal libyang's representation of a YANG data model. Each schema is connected with
 * its [context](@ref howtocontext) and loaded using [parser functions](@ref howtoschemasparsers). It means, that
 * the schema cannot be created (nor changed) programmatically. In libyang, schemas are used only to
 * access data model definitions.
 *
 * Schema tree nodes are able to hold private objects (via a pointer to a structure, function, variable, ...) used by
 * a caller application. Such an object can be assigned to a specific node using lys_set_private() function.
 * Note that the object is not freed by libyang when the context is being destroyed. So the caller is responsible
 * for freeing the provided structure after the context is destroyed or the private pointer is set to NULL in
 * appropriate schema nodes where the object was previously set. This can be automated via destructor function
 * to free these private objects. The destructor is passed to the ly_ctx_destroy() function. On the other hand,
 * freeing the object while the schema tree is still in use can lead to a segmentation fault.
 *
 * - @subpage howtoschemasparsers
 * - @subpage howtoschemasfeatures
 * - @subpage howtoschemaplugins
 * - @subpage howtoschemasprinters
 *
 * \note There are many functions to access information from the schema trees. Details are available in
 * the [Schema Tree module](@ref schematree).
 *
 * For information about difference between implemented and imported modules, see the
 * [context description](@ref howtocontext).
 *
 * Functions List (not assigned to above subsections)
 * --------------------------------------------------
 * - lys_getnext()
 * - lys_parent()
 * - lys_module()
 * - lys_node_module()
 * - lys_set_private()
 * - lys_set_implemented()
 * - lys_set_disabled()
 * - lys_set_enabled()
 */

/**
 * @page howtoschemasparsers Parsing Schemas
 *
 * Schema parser allows to read schema from a specific format. libyang supports the following schema formats:
 *
 * - YANG
 *
 *   Basic YANG schemas format described in [RFC 6020](http://tools.ietf.org/html/rfc6020) and
 *   [RFC 7951](http://tools.ietf.org/html/rfc7951) (so both YANG 1.0 and YANG 1.1 versions are supported).
 *
 * - YIN
 *
 *   Alternative XML-based format to YANG - YANG Independent Notation. The details can be found in
 *   [RFC 6020](http://tools.ietf.org/html/rfc6020#section-11) and
 *   [RFC 7951](http://tools.ietf.org/html/rfc7951#section-13).
 *
 * When the [context](@ref howtocontext) is created, it already contains the following schemas, which
 * are implemented internally by libyang:
 * - ietf-yang-metadata@2016-08-05
 * - yang@2017-02-20
 * - ietf-inet-types@2013-07-15
 * - ietf-yang-types@2013-07-15
 * - ietf-yang-library@2016-06-21
 *
 * The `yang` schema is the libyang's internal module to provide namespace and definitions of for various YANG
 * attributes described in [RFC 7951](https://tools.ietf.org/html/rfc6243) (such as `insert` attribute for
 * edit-config's data).
 *
 * Other schemas can be added to the context manually as described in [context page](@ref howtocontext) by the functions
 * listed below. Besides the schema parser functions, it is also possible to use ly_ctx_load_module() which tries to
 * find the required schema automatically - using #ly_module_imp_clb or automatic search in working directory and in the
 * context's searchpath.
 *
 * Functions List
 * --------------
 * - lys_parse_mem()
 * - lys_parse_fd()
 * - lys_parse_path()
 * - lys_search_localfile()
 * - ly_ctx_set_module_imp_clb()
 * - ly_ctx_load_module()
 */

/**
 * @page howtoschemasfeatures YANG Features Manipulation
 *
 * The group of functions prefixed by \b lys_features_ are used to access and manipulate with the schema's
 * features.
 *
 * The first two functions are used to access information about the features in the schema.
 * lys_features_list() provides list of all features defined in the specific schema and its
 * submodules. Optionally, it can also provide information about the state of all features.
 * Alternatively, caller can use lys_features_state() function to get state of one specific
 * feature.
 *
 * The remaining two functions, lys_features_enable() and lys_features_disable(), are used
 * to enable and disable the specific feature (or all via the '`*`' value). By default, when the module
 * is loaded by libyang parser, all features are disabled.
 *
 * To get know, if a specific schema node is currently disabled or enable, the lys_is_disabled() function can be used.
 *
 * Note, that the feature's state can affect some of the output formats (e.g. Tree format).
 *
 * Functions List
 * --------------
 * - lys_features_list()
 * - lys_features_enable()
 * - lys_features_disable()
 * - lys_features_state()
 * - lys_is_disabled()
 */

/**
 * @page howtoschemaplugins YANG Extension and User Type Support
 *
 * Extensions and user types are supported in the form of **plugins**. These are loaded from the plugin directory
 * (`LIBDIR/libyang/`) whenever a context is created. However, the list of plugins can be refreshed manually by ly_load_plugins().
 * The extension plugin directory path (default `LIBDIR/libyang/extensions/`) can be change via the
 * `LIBYANG_EXTENSIONS_PLUGINS_DIR` environment variable and similarly the user type directory (default `LIBDIR/libyang/user_types/`)
 * via `LIBYANG_USER_TYPES_PLUGINS_DIR`. Note, that unavailable plugins are not removed, only
 * any new plugins are loaded. Also note that the availability of new plugins does not affect the current schemas in the
 * contexts, they are applied only to the newly parsed schemas.
 *
 * The plugins list can be cleaned by ly_clean_plugins(). However, since various contexts (respectively their
 * schemas) can link to the plugins, the cleanup is successful only when there is no remaining context.
 *
 * For both plugin types, when a specific plugin is being loaded, libyang checks for the API version the plugin was compiled with.
 * From the plugin side, this is done using `LYEXT_VERSION_CHECK` or `LYTYPE_VERSION_CHECK` which must be defined in the plugin code.
 *
 * @section extensions Extensions
 *
 * YANG provides extensions as a mechanism how to add new statements into the language. Since they are very generic -
 * extension instance can appear anywhere, they can contain any other YANG statement including extension instances and
 * their specification has a form of the description text, it is very challenging for libyang to provide working and
 * useful implementation. Therefore, you can reach some limitation regarding the extensions support, but the common
 * use cases should be covered and supported.
 *
 * Since libyang does not understand human text, it is not possible to get the complete definition of the extension from
 * its description statement. Therefore, libyang allows the schema authors to provide @link extplugins extension
 * plugin@endlink that provides information from the extension description to libyang.
 *
 * Here are some notes about the implementation of the particular YANG extensions features
 *
 * - Extension instance can appear anywhere
 *   This is completely supported. In addition, the extension plugins are allowed to provide callback function to
 *   check if the extension instance is actually allowed to be instantiated as a substatement of the particular node.
 *   In some case the extension instance is not stored in the libyang structures inside the statement it is
 *   instantiated since some statements (such as description, position and several others listed as #LYEXT_SUBSTMT) are
 *   not stored as a structure, but directly as a value. The extension instances of such statements are then actually
 *   stored besides the extension instances of the parent structure (e.g. in ::lys_type_bit in case of position's
 *   extension instances). These extension instances are indicated by the ::lys_ext_instance#insubstmt value which
 *   refers (sometimes in combination with ::lys_ext_instance#insubstmt_index) to the substatement where the particular
 *   extension is actually instantiated.
 *
 * - Extension instance can contain any other YANG statement
 *   This is supported with exceptions - the list of YANG statements is defined as #LY_STMT enumeration with the
 *   description if the specific statement can appear as an extension instance substatement or if there is some
 *   limitation regarding its cardinality. It also specifies how the specific statement is stored in the extension
 *   instance (which data type or structure is used). In some cases, like `include` or `yang-version` statements, it
 *   make no sense to have them inside an extension instance, so they are not supported by libyang.
 *
 *   The list of allowed substatement for the specific extension instances is provided by the extension plugin. Besides
 *   the list of substatement, the plugin also specifies where the data are stored and the cardinality for each
 *   substatement. Other extension instances as an extension instance substatements are always allowed. Note, that if
 *   there is no extension plugin defined or if the list of the allowed substatements is empty, no substatement (except
 *   extension instances) is allowed.
 *
 * There are several extension types (enumerated as #LYEXT_TYPE) according to their purpose and content:
 *
 * - #LYEXT_FLAG - simple extension with no content (no substatement is allowed). The extension instance serves just as
 *   a flag with or without an argument and its value. The example of such an extension is NACM's __default-deny-write__
 *   or __default-deny-all__. This is the default type of extension for the case there is no extension plugin available
 *   for the specific extension. Extension instances of this type are represented as ::lys_ext_instance.
 *
 * - #LYEXT_COMPLEX - complex extension with content. The specific rules for the content and how it is stored in the
 *   extension instance structure is specified by the plugin. Extension instances of this type are represented as
 *   ::lys_ext_instance_complex.
 *
 * Note that the ::lys_ext_instance structure serves as a generic structure for all extension instances. In case the
 * ::lys_ext_instance#ext_type is set to a different value than #LYEXT_FLAG, the structure can be cast to the particular
 * extension instance structure to access the type-specific members.
 *
 * @subsection extplugins Extension Plugins
 *
 * Extension plugins provide more detailed information about the extension in a understandable form for libyang. These
 * information is usually provided in a text form in the extension's description statement. libyang provides several
 * plugins for the common IETF extensions (NACM, Metadata, ...) that can be used as a code examples for other
 * extensions.
 *
 * There are several types of extension plugins which corresponds to the extension types:
 *
 * - ::lyext_plugin - the plugin for a simple extensions of type #LYEXT_FLAG. The plugin is supposed to provide
 *   callbacks for:
 *   + @link lyext_check_position_clb checking extension position @endlink (if the extension instance can be instantiated
 *     in a particular parent)
 *   + @link lyext_check_inherit_clb deciding if the extension is supposed to be inherited @endlink
 *     (see #LYEXT_OPT_INHERIT). Note that the inherited extension instances are available in the libyang data
 *     structures, but they are not part of the schema so they are not printed into the schema output formats.
 *   + @link lyext_check_result_clb checking the result @endlink, the callback is called when the schema is completely
 *     parsed and all the data are available for the final (possibly complex) checking (or modification)
 * - ::lyext_plugin_complex - the plugin for the complex extension instances (#LYEXT_COMPLEX). The structure extends
 *   the ::lyext_plugin structure by:
 *   + instance_size - the overall size of the ::lyext_plugin_complex structure to allocate.
 *     The substatement list is flexible array, so the size of the plugin structure depends on the content of the array.
 *   + substmt - the list of the allowed substatements to the extension instance, besides the substatement id, it
 *     also specifies its cardinality and the specific position in the ::lys_ext_instance_complex where the data are
 *     stored (as offset to the ::lys_ext_instance_complex#content member). The way how the data are stored is
 *     specified descriptions of #LY_STMT values.
 *
 * @subsection howtoschemametadata Metadata Support
 *
 * YANG Metadata annotations are defined in [RFC 7952](https://tools.ietf.org/html/rfc6243) as YANG extension. In
 * practice, it allows to have XML attributes (there is also a special encoding for JSON) in YANG modeled data.
 * libyang does not allow to have any XML attribute without the appropriate annotation definition describing the
 * data as it is done e.g. for leafs. When an attribute without a matching annotation definition is found in the
 * input data, it is silently dropped (with warning) or an error is reported in case the #LYD_OPT_STRICT parser
 * option is provided to the [parser function](@ref howtodataparsers).
 *
 * There are some XML attributes, described by [YANG](https://tools.ietf.org/html/rfc7951) and
 * [NETCONF](https://tools.ietf.org/html/rfc6241) specifications, which are not defined as annotations, but libyang
 * implements them this way. In case of attributes in the YANG namespace (`insert`, `value` and `key` attributes
 * for the NETCONF edit-config operation), they are defined in special libyang's internal module `yang`, which is
 * available in each context and the content of this schema can be printed via
 * [schema printers](@ref howtoschemasprinters).
 *
 * In case of the attributes described in [NETCONF specification](https://tools.ietf.org/html/rfc6241), the libyang's
 * annotations structures are hidden and cannot be printed despite, internally, they are part of the `ietf-netconf`'s
 * schema structure. Therefore, these attributes are available only when the `ietf-netconf` schema is loaded in the
 * context. The definitions of these annotations are as follows:
 *
 *     md:annotation operation {
 *       type enumeration {
 *         enum merge;
 *         enum replace;
 *         enum create;
 *         enum delete;
 *         enum remove;
 *       }
 *     }
 *
 *     md:annotation type {
 *       type enumeration {
 *         enum subtree;
 *         enum xpath {
 *           if-feature "nc:xpath";
 *         }
 *       }
 *     }
 *
 *     md:annotation select {
 *       type string;
 *     }
 *
 * Note, that, following the specification,
 * - the `type` and `select` XML attributes are supposed to be unqualified (without namespace) and that
 * - the `select`'s content is XPath and it is internally transformed by libyang into the format where the
 *   XML namespace prefixes are replaced by the YANG module names.
 *
 * @section usertypes User Types
 *
 * Using this plugin mechanism, it is also possible to define what can be called **user types**. Values are
 * always stored as a string in addition to being in a #lyd_val union. It is possible to customize how
 * the value is stored in the union using a #lytype_store_clb callback.
 *
 * Generally, it is meant for storing certain types more effectively. For instance, when working with **ipv4-address**
 * from the *ietf-inet-types* model, an application will most likely use the address in a binary form, not as a string.
 * So, in the callback the value is simply transformed into the desired format and saved into #lyd_val value. However,
 * the callback is allowed to store anything in the union. Another example, if there are many strings being created and
 * handled, is to store the string length instead having 2 pointers to the same string.
 *
 * Also, it is possible to perform some additional validation of the value except for the standard YANG one. Even if
 * the value should only be validated, this callback can be defined and used, it will just keep the #lyd_val value
 * unchanged.
 *
 * @subsection typeplugins User Type Plugins
 *
 * There are simple example user type plugins in `src/user_types`.
 *
 * - ::lytype_plugin_list - plugin is supposed to provide callbacks for:
 *   + @link lytype_store_clb storing the value itself @endlink
 *   + freeing the stored value (optionally, if the store callback allocates memory)
 *
 * Functions List
 * --------------
 * - lys_ext_instance_presence()
 * - lys_ext_instance_substmt()
 * - ly_load_plugins()
 * - ly_clean_plugins()
 */

/**
 * @page howtoschemasprinters Printing Schemas
 *
 * Schema printers allows to serialize internal representation of a schema module in a specific format. libyang
 * supports the following schema formats for printing:
 *
 * - YANG
 *
 *   Basic YANG schemas format described in [RFC 6020](http://tools.ietf.org/html/rfc6020) and
 *   [RFC 7951](http://tools.ietf.org/html/rfc7951) (so both YANG 1.0 and YANG 1.1 versions are supported).
 *
 * - YIN
 *
 *   Alternative XML-based format to YANG - YANG Independent Notation. The details can be found in
 *   [RFC 6020](http://tools.ietf.org/html/rfc6020#section-11) and
 *   [RFC 7951](http://tools.ietf.org/html/rfc7951#section-13).
 *
 * - Tree
 *
 *   Simple tree structure of the module where each node is printed as:
 *
 *       <status> <flags> <name> <opts> <type> <if-features>
 *
 *   - `<status>` is one of:
 *     - `+` for current
 *     - `x` for deprecated
 *     - `o` for obsolete
 *
 *   - `<flags>` is one of:
 *     - `rw` for configuration data
 *     - `ro` for status data
 *     - `-x` for RPCs
 *     - `-n` for Notification
 *
 *   - `<name>` is the name of the node
 *     - `(<name>)` means that the node is a choice node
 *     - `:(<name>)` means that the node is a case node
 *     - if the node is augmented into the tree from another module, it is printed with the module name as
 *     `<module-name>:<name>`.
 *
 *   - `<opts>` is one of:
 *     - `?` for an optional leaf or choice
 *     - `!` for a presence container
 *     - `*` for a leaf-list or list
 *     - `[<keys>]` for a list's keys
 *
 *   - `<type>` is the name of the type for leafs and leaf-lists
 *     - if there is a default value defined, it is printed within angle brackets `<default-value>`
 *     - if the type is a leafref, the type is printed as -> TARGET`
 *
 *   - `<if-features>` is the list of features this node depends on, printed within curly brackets and
 *     a question mark `{...}?`
 *
 *
 * - Info
 *
 *   Detailed information about the specific node in the schema tree.
 *   It allows to print information not only about a specific module, but also about its specific part:
 *
 *   - absolute-schema-nodeid
 *
 *     e.g. \a `/modules/module-set-id`  in \a `ietf-yang-library` module
 *
 *   - <b>typedef/</b>typedef-name
 *
 *     e.g. \a `typedef/revision-identifier` in \a `ietf-yang-library` module
 *
 *   - <b>feature/</b>feature-name
 *
 *     e.g. \a `feature/ssh` in \a `ietf-netconf-server` module
 *
 *   - <b>grouping/</b>grouping-name/descendant-schema-nodeid
 *
 *     e.g. \a `grouping/module` or \a `grouping/module/module/submodules` in \a `ietf-yang-library` module
 *
 *   - <b>type/</b>leaf-or-leaflist
 *
 *     e.g. \a `type/modules/module-set-id` in \a `ietf-yang-library` module
 *
 * Printer functions allow to print to the different outputs including a callback function which allows caller
 * to have a full control of the output data - libyang passes to the callback a private argument (some internal
 * data provided by a caller of lys_print_clb()), string buffer and number of characters to print. Note that the
 * callback is supposed to be called multiple times during the lys_print_clb() execution.
 *
 * Functions List
 * --------------
 * - lys_print_mem()
 * - lys_print_fd()
 * - lys_print_file()
 * - lys_print_path()
 * - lys_print_clb()
 */

/**
 * @page howtodata Data Instances
 *
 * All data nodes in data trees are connected with their schema node - libyang is not able to represent data of an
 * unknown schema.
 *
 * Connected to the aforementioned, it is important to mention that there are several kinds of data trees libyang
 * can work with. Except for a standard full data tree and configuration data tree, there are trees specific for use
 * in NETCONF, which are \<get\> or \<get-config\> replies, \<edit-config\> input content, generic RPC/action request,
 * RPC/action reply, or a notification.
 *
 * Special care is needed when working with action input and RPC/action output, because in these cases the libyang data
 * tree does not match the actual data tree NETCONF expects.
 *
 * <b>Action input</b> differentiates from a standard RPC that the child of \<rpc\> element is not the specific RPC
 * request being sent, but a special \<action\> element in the base YANG namespace. It is expected to be present when
 * parsing an action input and can optionally be included when the action is printed.
 *
 * As for <b>RPC output</b>, NETCONF expects the output parameters as direct children of the \<rpc-reply\> element.
 * However, libyang encapsulates these parameters in the specific RPC request container. Same as before, it is expected
 * to be without this container when parsing and can be printed out.
 *
 * Lastly, <b>action output</b> is almost similar to RPC output and in NETCONF you actually cannot distinguish them,
 * but in case of action output, libyang puts all the output parameters under the action container, which is also
 * just a child to all the schema tree ancestors up to the schema top-level, just like in an action input. Same applies
 * even in this case, parsed data should be only the output parameters, you can decide how to print it.
 *
 * So, you do not have to use any special flag when you want to print the data tree the way libyang internally stores
 * it, although you will not be able to parse it back, as was mentioned in the paragraphs above. To print data in the
 * way NETCONF and libyang expects them to be, use #LYP_NETCONF flag for any printing function.
 *
 * Please, continue reading a specific subsection or go through all the subsections if you are a new user of libyang.
 *
 * - @subpage howtodataparsers
 * - @subpage howtodatamanipulators
 * - @subpage howtodatavalidation
 * - @subpage howtodatawd
 * - @subpage howtodataprinters
 *
 * \note API for this group of functions is described in the [Data Instances module](@ref datatree).
 *
 * Functions List (not assigned to above subsections)
 * --------------------------------------------------
 * - lyd_find_instance()
 * - lyd_find_xpath()
 * - lyd_leaf_type()
 */

/**
 * @page howtodataparsers Parsing Data
 *
 * Data parser allows to read instances from a specific format. libyang supports the following data formats:
 *
 * - XML
 *
 *   Original data format used in NETCONF protocol. XML mapping is part of the YANG specification
 *   ([RFC 6020](http://tools.ietf.org/html/rfc6020)).
 *
 * - JSON
 *
 *   The alternative data format available in RESTCONF protocol. Specification of JSON encoding of data modeled by YANG
 *   can be found in [RFC 7951](http://tools.ietf.org/html/rfc7951).
 *
 * Besides the format of input data, the parser functions accepts additional [options](@ref parseroptions) to specify
 * how the input data should be processed.
 *
 * In contrast to the schema parser, data parser also accepts empty input data if such an empty data tree is valid
 * according to the schemas in the libyang context.
 *
 * If a node from a schema, which is not present in the context or is not implemented, is parsed, a callback set by
 * ly_ctx_set_module_data_clb() is called and this way the application is given the opportunity to add this schema into
 * the context or change its conformance before the parsing would fail.
 *
 * In case of XML input data, there is one additional way to parse input data. Besides parsing the data from a string
 * in memory or a file, caller is able to build an XML tree using [libyang XML parser](@ref howtoxml) and then use
 * this tree (or a part of it) as input to the lyd_parse_xml() function.
 *
 * Functions List
 * --------------
 * - lyd_parse_mem()
 * - lyd_parse_fd()
 * - lyd_parse_path()
 * - lyd_parse_xml()
 */

/**
 * @page howtodatamanipulators Manipulating Data
 *
 * There are many functions to create or modify an existing data tree. You can add new nodes, reconnect nodes from
 * one tree to another (or e.g. from one list instance to another) or remove nodes. The functions doesn't allow you
 * to put a node to a wrong place (by checking the module), but not all validation checks can be made directly
 * (or you have to make a valid change by multiple tree modifications) when the tree is being changed. Therefore,
 * there is lyd_validate() function supposed to be called to make sure that the current data tree is valid. If
 * working with RPCs, they are invalid also in case the data nodes are not ordered according to the schema, which
 * you can fix easily with lyd_schema_sort(). Note, that not performing validation after some data tree changes
 * can cause failure of various libyang functions later. All functions performing data tree changes are marked
 * with the __PARTIAL CHANGE__ flag in their documentation meaning they leave at least partly non-validated data tree.
 *
 * Creating data is generally possible in two ways, they can be combined. You can add nodes one-by-one based on
 * the node name and/or its parent (lyd_new(), \b lyd_new_anydata_*(), lyd_new_leaf(), and their output variants) or
 * address the nodes using a simple XPath addressing (lyd_new_path()). The latter enables to create a whole path
 * of nodes, requires less information about the modified data, and is generally simpler to use. The path format
 * specifics can be found [here](@ref howtoxpath).
 *
 * Working with two data subtrees can also be performed two ways. Usually, you would use lyd_insert*() functions.
 * They are generally meant for simple inserts of a node into a data tree. For more complicated inserts and when
 * merging 2 trees use lyd_merge(). It offers additional options and is basically a more powerful insert.
 *
 * Also remember, that when you are creating/inserting a node, all the objects in that operation must belong to the
 * same context.
 *
 * Modifying the single data tree in multiple threads is not safe.
 *
 * Functions List
 * --------------
 * - lyd_dup()
 * - lyd_dup_to_ctx()
 * - lyd_change_leaf()
 * - lyd_insert()
 * - lyd_insert_sibling()
 * - lyd_insert_before()
 * - lyd_insert_after()
 * - lyd_insert_attr()
 * - lyd_merge()
 * - lyd_merge_to_ctx()
 * - lyd_new()
 * - lyd_new_anydata()
 * - lyd_new_leaf()
 * - lyd_new_path()
 * - lyd_new_output()
 * - lyd_new_output_anydata()
 * - lyd_new_output_leaf()
 * - lyd_list_pos()
 * - lyd_schema_sort()
 * - lyd_unlink()
 * - lyd_free()
 * - lyd_free_attr()
 * - lyd_free_withsiblings()
 */

/**
 * @page howtodatavalidation Validating Data
 *
 * By default, the represented data are supposed to represent a full YANG datastore content. So if a schema declares
 * some mandatory nodes, despite configuration or status, the data are supposed to be present in the data tree being
 * loaded or validated. However, it is possible to specify other kinds of data (see @ref parseroptions) allowing some
 * exceptions to the validation process.
 *
 * Data validation is performed implicitly to the input data processed by the parser (\b lyd_parse_*() functions) and
 * on demand via the lyd_validate() function. The lyd_validate() is supposed to be used when a (complex or simple)
 * change is done on the data tree (via a combination of \b lyd_change_*(), \b lyd_insert*(), \b lyd_new*(),
 * lyd_unlink() and lyd_free() functions).
 *
 * Part of data validation is resolving leafrefs and instance-identifiers. Leafrefs are resolved only when a change occurred
 * in the data tree that could have broken the link. However, as instance-identifiers can point to any node whatsoever
 * without an import, it would not be effective to store metadata as in the case of leafrefs. That is why they are resolved
 * during every validation. Also, for the same reason, it can easily happen that when parsing/validating data with
 * an instance-identifier, it will target a remote node, whose schema is not currently present in the context. To handle
 * this case, a callback should be set using ly_ctx_set_module_data_clb(), which can load the schema when required.
 *
 * Must And When Conditions Accessible Tree
 * ----------------------------------------
 *
 * In YANG 1.1, there can be \b must and/or \b when expressions in RPC/action input or output, or in notifications that
 * require access to the configuration datastore and/or state data. Normally, when working with any of the aforementioned
 * data trees, they must contain only the RPC/action/notification itself, without any additional configuration or state
 * data. So how can then these conditions be verified during validation?
 *
 * There is an option to pass this additional data tree to all the functions that perform \b must and \b when condition
 * checking (\b lyd_parse_*() and lyd_validate()). Also, there are 3 flags of \b struct lys_node that
 * mark schema nodes that include paths that require foreign nodes (outside their subtree) for their evaluation.
 * #LYS_XPCONF_DEP marks nodes with such must and/or when expressions that require some configuration data,
 * #LYS_XPSTATE_DEP that require some state data, and #LYS_LEAFREF_DEP with such a leafref.
 * The subtree root is always the particular operation data node (for RPC it is the RPC data node and all
 * the input or output nodes as its children and similarly for action and notification). Note that for action and
 * not-top-level notification this means that all their parents are not considered as belonging to their subtree even though
 * they are included in their data tree and must be present for the operation validation to pass. The reason for this is that if
 * there are any lists in those parents, we cannot know if there are not some other instances of them in the standard
 * data tree in addition to the one used in the action/notification invocation.
 *
 * There were 2 ways of using this mechanism envisioned (explained below), but you can combine or modify them.
 *
 * ### Fine-grained Data Retrieval ###
 *
 * This approach is recommended when you do not maintain a full configuration data tree with state data at all times.
 *
 * Firstly, you should somehow learn that the operation data tree you are currently working with includes some schema
 * node instances that have conditions that require foreign data. You can either know this about every operation beforehand
 * or you go through all the schema nodes looking for the flags #LYS_XPCONF_DEP, #LYS_XPSTATE_DEP, and #LYS_LEAFREF_DEP.
 * Then you should use lys_node_xpath_atomize()
 * to retrieve all XPath condition dependencies (in the form of schema nodes) outside the operation subtree. You will likely
 * want to use the flag #LYXP_NO_LOCAL to get rid of all the nodes from inside the subtree (you should already have those).
 * The last thing to do is to build a data tree that includes at least all the instances of the nodes obtained from lys_node_xpath_atomize()
 * (it will be expected). Then you pass this tree to the validation and it should now have access to all the nodes that
 * can potentially affect the XPath evaluation and no other.
 *
 * ### Maintaining Configuration And State Data Tree ###
 *
 * If you have a full data tree with state data available for the validation process then it is quite simple (compared
 * to the first approach). You can simply always pass it to validation of these operations and in cases it is not required
 * (no nodes with conditions traversing foreign nodes) only a negligible amount of redundant work is performed and you can
 * skip the process of learning whether it is required or not.
 *
 * Functions List
 * --------------
 * - lyd_validate()
 */

/**
 * @page howtodatawd Default Values
 *
 * libyang provides support for work with default values as defined in [RFC 6243](https://tools.ietf.org/html/rfc6243).
 * This document defines 4 modes for handling default nodes in a data tree, libyang adds the fifth mode:
 * - \b explicit - Only the explicitly set configuration data. But in the case of status data, missing default
 *                 data are added into the tree. In libyang, this mode is represented by #LYP_WD_EXPLICIT option.
 * - \b trim - Data nodes containing the schema default value are removed. This mode is applied using #LYP_WD_TRIM option.
 * - \b report-all - All the missing default data are added into the data tree. This mode is represented by
 *                 #LYP_WD_ALL option.
 * - \b report-all-tagged - In this case, all the missing default data are added as in case of the `report-all` mode,
 *                 but additionally all the nodes (existing as well as added) containing the schema default value
 *                 are tagged (see the note below). libyang uses #LYP_WD_ALL_TAG option for this mode.
 * - \b report-implicit-tagged - The last mode is similar to the previous one, except only the added nodes are tagged.
 *                 This is the libyang's extension and it is represented by #LYP_WD_IMPL_TAG option.
 *
 * libyang automatically adds/maintain the default nodes when a data tree is being parsed or validated. Note, that in a
 * modified data tree (via e.g. lyd_insert() or lyd_free()), some of the default nodes can be missing or they can be
 * present by mistake. Such a data tree is again corrected during the next lyd_validate() call.
 *
 * The implicit (default) nodes, created by libyang, are marked with the ::lyd_node#dflt flag which applies to the
 * leafs and leaf-lists. In case of containers, the flag means that the container holds only a default node(s) or it
 * is an empty container (according to YANG 1.1 spec, all such containers are part of the accessible data tree).
 *
 * The presence of the default nodes during the data tree lifetime is affected by the LYD_OPT_ flag used to
 * parse/validate the tree:
 * - #LYD_OPT_DATA - all the default nodes are present despite they are configuration or status nodes
 * - #LYD_OPT_CONFIG - only the configuration data nodes are added into the tree
 * - #LYD_OPT_GET, #LYD_OPT_GETCONFIG, #LYD_OPT_EDIT - no default nodes are added
 * - #LYD_OPT_RPC, #LYD_OPT_RPCREPLY, #LYD_OPT_NOTIF - the default nodes from the particular subtree are added
 *
 * The with-default modes described above are supported when the data tree is being printed with the
 * [LYP_WD_ printer flags](@ref printerflags). Note, that in case of #LYP_WD_ALL_TAG and #LYP_WD_IMPL_TAG modes,
 * the XML/JSON attributes are printed only if the context includes the ietf-netconf-with-defaults schema. Otherwise,
 * these modes have the same result as #LYP_WD_ALL. The presence of empty containers (despite they were added explicitly
 * or implicitly as part of accessible data tree) depends on #LYP_KEEPEMPTYCONT option.
 *
 * To get know if the particular leaf or leaf-list node contains default value (despite implicit or explicit), you can
 * use lyd_wd_default() function.
 *
 * Functions List
 * --------------
 * - lyd_wd_default()
 *
 * - lyd_parse_mem()
 * - lyd_parse_fd()
 * - lyd_parse_path()
 * - lyd_parse_xml()
 * - lyd_validate()
 * - lyd_print_mem()
 * - lyd_print_fd()
 * - lyd_print_file()
 * - lyd_print_path()
 * - lyd_print_clb()
 */

/**
 * @page howtodataprinters Printing Data
 *
 * Data printers allows to serialize internal representation of a data tree in a specific format. libyang
 * supports the following data formats for printing:
 *
 * - XML
 *
 *   Basic format as specified in rules of mapping YANG modeled data to XML in
 *   [RFC 6020](http://tools.ietf.org/html/rfc6020). It is possible to specify if
 *   the indentation (formatting) will be used (by #LYP_FORMAT @ref printerflags "printer option").
 *
 * - JSON
 *
 *   The alternative data format available in RESTCONF protocol. Specification of JSON encoding of data modeled by YANG
 *   can be found in [this draft](https://tools.ietf.org/html/draft-ietf-netmod-yang-json-05).It is possible to specify
 *   if the indentation (formatting) will be used (by #LYP_FORMAT @ref printerflags "printer option").
 *
 * Printer functions allow to print to the different outputs including a callback function which allows caller
 * to have a full control of the output data - libyang passes to the callback a private argument (some internal
 * data provided by a caller of lyd_print_clb()), string buffer and number of characters to print. Note that the
 * callback is supposed to be called multiple times during the lyd_print_clb() execution.
 *
 * To print the data tree with default nodes according to the with-defaults capability defined in
 * [RFC 6243](https://tools.ietf.org/html/rfc6243), check the [page about the default values](@ref howtodatawd).
 *
 * Also, to print the data in NETCONF format, use the #LYP_NETCONF flag. More information can be found on the page
 * @ref howtodata.
 *
 * Functions List
 * --------------
 * - lyd_print_mem()
 * - lyd_print_fd()
 * - lyd_print_file()
 * - lyd_print_clb()
 */

/**
 * @page howtoxpath XPath Addressing
 *
 * Internally, XPath evaluation is performed on __when__ and __must__ conditions in the schema. For that almost
 * a full XPath 1.0 evaluator was implemented. In YANG models you can also find paths identifying __augment__
 * targets, __leafref__ targets, and trivial paths in __choice default__ and __unique__ statements argument.
 * The exact format of all those paths can be found in the relevant RFCs. Further will only be discussed
 * paths that are used directly in libyang API functions.
 *
 * Schema
 * ======
 *
 * Regarding identifying schema nodes, we use a slightly modified version of YANG __augment__ target path:
 *   - strictly speaking, most XPath expressions are not accepted, only simple paths (no predicates,
 *     numbers, literals, operators, ...),
 *   - whenever a prefix is used for a node, it is not the import prefix, but the __module name__ itself,
 *   - __current module__ is specified separately for _absolute_ paths and is the module of the start
 *     (current) node for _relative_ paths,
 *   - unprefixed nodes all use the prefix of the __current module__ so all nodes from other modules than
 *     the __current module__ _MUST_ have prefixes,
 *   - nodes from the __current module__ _MAY_ have prefixes,
 *
 * Examples
 * --------
 *
 * - get all top-level nodes of the __module-name__
 *
 *       /module-name:*
 *
 * - get all the descendants of __container__ (excluding __container__)
 *
 *       /module-name:container//\asterisk
 *
 * - get __aug-leaf__, which was added to __module-name__ from an augment module __augment-module__
 *
 *       /module-name:container/container2/augment-module:aug-cont/augment-module:aug-leaf
 *
 * Functions List
 * --------------
 * - lys_find_path()
 * - lys_path()
 * - ly_path_data2schema()
 *
 *
 * Data
 * ====
 *
 * As for paths evaluated on YANG data, we opted for standardized JSON paths ([RFC 7951](https://tools.ietf.org/html/rfc7951#section-6.11)). Summarized, it follows these conventions:
 *   - generally, you can use almost a full XPath in these paths where it makes sense, but only data nodes (node sets)
 *     will always be returned (except for paths, predicates are mostly used),
 *   - as per the specification, prefixes are actually __module names__,
 *   - also in the specification, for _absolute_ paths, the first (leftmost) node _MUST_ have a prefix,
 *   - for _relative_ paths, you specify the __context node__, which then acts as a parent for the first node in the path,
 *   - nodes always inherit their module (prefix) from their __parent node__ so whenever a node is from a different
 *     module than its parent, it _MUST_ have a prefix,
 *   - nodes from the same module as their __parent__ _MUST NOT_ have a prefix,
 *   - different from schema paths, non-data nodes (choice, case, uses, input, output) are skipped and not included
 *     in the path.
 *
 * Examples
 * --------
 *
 * - get __list__ instance with __key1__ of value __1__ and __key2__ of value __2__ (this can return more __list__ instances if there are more keys than __key1__ and __key2__)
 *
 *       /module-name:container/list[key1='1'][key2='2']
 *
 * - get __leaf-list__ instance with the value __val__
 *
 *       /module-name:container/leaf-list[.='val']
 *
 * - get __aug-list__ with __aug-list-key__, which was added to __module-name__ from an augment module __augment-module__
 *
 *       /module-name:container/container2/augment-module:aug-cont/aug-list[aug-list-key='value']
 *
 * Functions List
 * --------------
 * - lyd_find_path()
 * - lyd_new_path()
 * - lyd_path()
 * - lys_data_path()
 * - ly_ctx_get_node()
 * - ly_ctx_find_path()
 *
 */

/**
 * @page howtoxml libyang XML Support
 *
 * libyang XML parser is able to parse XML documents. The main purpose is to load data modeled by YANG. However, it can
 * be used as a standalone XML parser with the following limitations in comparison to a full-featured XML parsers:
 * - comments are ignored
 * - Doctype declaration is ignored
 * - CData sections are ignored
 * - Process Instructions (PI) are ignored
 *
 * The API is designed to almost only read-only access. You can simply load XML document, go through the tree as
 * you wish and dump the tree to an output. The only "write" functions are lyxml_free() and lyxml_unlink() to remove
 * part of the tree or to unlink (separate) a subtree.
 *
 * XML parser is used internally by libyang for parsing YIN schemas and data instances in XML format.
 *
 * \note API for this group of functions is described in the [XML Parser module](@ref xmlparser).
 *
 * Functions List
 * --------------
 * - lyxml_parse_mem()
 * - lyxml_parse_path()
 * - lyxml_get_attr()
 * - lyxml_get_ns()
 * - lyxml_print_mem()
 * - lyxml_print_fd()
 * - lyxml_print_file()
 * - lyxml_print_clb()
 * - lyxml_unlink()
 * - lyxml_free()
 */

/**
 * @page howtothreads libyang in Threads
 *
 * libyang can be used in multithreaded applications keeping in mind the following rules:
 * - libyang context manipulation (adding new schemas, removing schemas or even printing schemas) is not thread safe
 *   and it is supposed to be done in a main thread before any other work with context, schemas or data instances.
 *   Destroying the context is supposed to be done when no other thread accesses context, schemas nor data trees,
 * - data parser (\b lyd_parse*() functions) can be used simultaneously in multiple threads (also the returned
 *   #ly_errno is thread safe),
 * - data manipulation (lyd_new(), lyd_insert(), lyd_unlink(), lyd_free() and many other
 *   functions) a single data tree is not thread safe,
 * - data printing of a single data tree is thread-safe.
 */

/**
 * @page howtologger Logger
 *
 * There are 4 verbosity levels defined as ::LY_LOG_LEVEL. The level can be
 * changed by the ly_verb() function. By default, the verbosity level is
 * set to #LY_LLERR value.
 *
 * All the logging operations are tied to the specific **thread** and **context**.
 * The default behaviour is that the last message (error or warning, verbose and debug
 * messages are never stored) is always stored and can be accessed using ly_errmsg(). On error,
 * #ly_errno is set. If that was a validation error (#ly_errno is set to #LY_EVALID),
 * also validation error code (via ly_vecode()) and path to the error node (via ly_errpath()) are available.
 *
 * For some specific cases, a YANG schema can define error message and/or error tag (mainly for
 * use in NETCONF). If a message is set, it is provided via ly_errmsg(). If a tag is set in schema,
 * it is available via ly_errapptag() (if not set, the returned string is empty).
 *
 * By default, all libyang messages are printed to `stderr`. However, the caller is able to set their own logging
 * callback function. In that case, instead of printing messages, libyang passes error level, message and path
 * (if any) to the caller's callback function. In case of error level, the message and path are still
 * automatically stored and available via the functions and macros described above.
 *
 * This is the basic way of working with errors but another, more sophisticated is also available. With ly_log_options()
 * you can modify what is done with all the messages. Default flags are #LY_LOLOG and #LY_LOSTORE_LAST so that messages
 * are logged and the last one is stored. If you set the flag #LY_LOSTORE, all the messages will be stored. Be careful
 * because unless you regularly clean them, the error list will grow indefinitely. With ly_err_first() you can retrieve
 * the first generated error structure ly_err_item. It is a linked-list so you can get next errors using the **next** pointer.
 * Being processed (for instance printed with ly_err_print()), you can then free them with ly_err_clean().
 *
 * \note API for this group of functions is described in the [logger module](@ref logger).
 *
 * Functions List
 * --------------
 * - ly_verb()
 * - ly_set_log_clb()
 * - ly_get_log_clb()
 * - ly_log_options()
 * - #ly_errno
 * - ly_vecode()
 * - ly_errmsg()
 * - ly_errpath()
 * - ly_errapptag()
 * - ly_err_first()
 * - ly_err_print()
 * - ly_err_clean()
 */

/**
 * @defgroup context Context
 * @{
 *
 * Structures and functions to manipulate with the libyang "containers". The \em context concept allows callers
 * to work in environments with different sets of YANG schemas. More detailed information can be found at
 * @ref howtocontext page.
 */

/**
 * @struct ly_ctx
 * @brief libyang context handler.
 */
struct ly_ctx;

/**
 * @defgroup contextoptions Context options
 * @ingroup context
 *
 * Options to change context behavior.
 * @{
 */

#define LY_CTX_ALLIMPLEMENTED 0x01 /**< All the imports of the schema being parsed are treated implemented. */
#define LY_CTX_TRUSTED        0x02 /**< Handle the schema being parsed as trusted and skip its validation
                                        tests. Note that while this option improves performance, it can
                                        lead to an undefined behavior if the schema is not correct. */
#define LY_CTX_NOYANGLIBRARY  0x04 /**< Do not internally implement ietf-yang-library module. The option
                                        causes that function ly_ctx_info() does not work (returns NULL) until
                                        the ietf-yang-library module is loaded manually. While any revision
                                        of this schema can be loaded with this option, note that the only
                                        revisions implemented by ly_ctx_info() are 2016-04-09 and 2018-01-17.
                                        This option cannot be used with ly_ctx_new_yl*() functions. */
#define LY_CTX_DISABLE_SEARCHDIRS 0x08  /**< Do not search for schemas in context's searchdirs neither in current
                                        working directory. It is entirely skipped and the only way to get
                                        schema data for imports or for ly_ctx_load_module() is to use the
                                        callbacks provided by caller via ly_ctx_set_module_imp_clb() */
#define LY_CTX_DISABLE_SEARCHDIR_CWD 0x10 /**< Do not automatically search for schemas in current working
                                        directory, which is by default searched automatically (despite not
                                        recursively). */
#define LY_CTX_PREFER_SEARCHDIRS 0x20 /**< When searching for schema, prefer searchdirs instead of user callback. */
/**@} contextoptions */

/**
 * @brief Create libyang context
 *
 * Context is used to hold all information about schemas. Usually, the application is supposed
 * to work with a single context in which libyang is holding all schemas (and other internal
 * information) according to which the data trees will be processed and validated. So, the schema
 * trees are tightly connected with the specific context and they are held by the context internally
 * - caller does not need to keep pointers to the schemas returned by lys_parse(), context knows
 * about them. The data trees created with lyd_parse() are still connected with the specific context,
 * but they are not internally held by the context. The data tree just points and lean on some data
 * held by the context (schema tree, string dictionary, etc.). Therefore, in case of data trees, caller
 * is supposed to keep pointers returned by the lyd_parse() and manage the data tree on its own. This
 * also affects the number of instances of both tree types. While you can have only one instance of
 * specific schema connected with a single context, number of data tree instances is not connected.
 *
 * @param[in] search_dir Directory where libyang will search for the imported or included modules
 * and submodules. If no such directory is available, NULL is accepted.
 * @param[in] options Context options, see @ref contextoptions.
 *
 * @return Pointer to the created libyang context, NULL in case of error.
 */
struct ly_ctx *ly_ctx_new(const char *search_dir, int options);

/**
 * @brief Create libyang context according to the content of the given yang-library data.
 *
 * This function loads the yang-library data from the file at the given path. If you need
 * to pass the data as string, use ly_ctx_new_ylmem(). Both functions extend functionality of
 * ly_ctx_new() by loading modules specified in the ietf-yang-library form into the context
 * being created.
 *
 * The preferred tree model revision is 2018-01-17. However, only the first module-set is processed
 * and loaded into the context. If there are no matching nodes from this tree, the legacy tree
 * (originally from model revision 2016-04-09) is processed.
 *
 * Note, that the modules are loaded the same way as in case of ly_ctx_load_module(), so the schema
 * paths in the yang-library data are ignored and the modules are loaded from the context's search
 * locations. On the other hand, YANG features of the modules are set as specified in the yang-library
 * data.
 *
 * To get yang library data from a libyang context, use ly_ctx_info().
 *
 * @param[in] search_dir Directory where libyang will search for the imported or included modules
 * and submodules. If no such directory is available, NULL is accepted.
 * @param[in] path Path to the file containing yang-library data in the specified \p format.
 * @param[in] format Format of the data in the provided file.
 * @param[in] options Context options, see @ref contextoptions.
 * @return Pointer to the created libyang context, NULL in case of error.
 */
struct ly_ctx *ly_ctx_new_ylpath(const char *search_dir, const char *path, LYD_FORMAT format, int options);

/**
 * @brief Create libyang context according to the content of the given yang-library data.
 *
 * This function loads the yang-library data from the given string. If you need to pass the data
 * as path to a file holding the data, use ly_ctx_new_ylpath(). Both functions extend functionality of
 * ly_ctx_new() by loading modules specified in the ietf-yang-library form into the context
 * being created.
 *
 * The preferred tree model revision is 2018-01-17. However, only the first module-set is processed
 * and loaded into the context. If there are no matching nodes from this tree, the legacy tree
 * (originally from model revision 2016-04-09) is processed.
 *
 * Note, that the modules are loaded the same way as in case of ly_ctx_load_module(), so the schema
 * paths in the yang-library data are ignored and the modules are loaded from the context's search
 * locations. On the other hand, YANG features of the modules are set as specified in the yang-library
 * data.
 *
 * To get yang library data from a libyang context, use ly_ctx_info().
 *
 * @param[in] search_dir Directory where libyang will search for the imported or included modules
 * and submodules. If no such directory is available, NULL is accepted.
 * @param[in] data String containing yang-library data in the specified \p format.
 * @param[in] format Format of the data in the provided file.
 * @param[in] options Context options, see @ref contextoptions.
 * @return Pointer to the created libyang context, NULL in case of error.
 */
struct ly_ctx *ly_ctx_new_ylmem(const char *search_dir, const char *data, LYD_FORMAT format, int options);

/**
 * @brief Number of internal modules, which are in the context and cannot be removed nor disabled.
 * @param[in] ctx Context to investigate.
 * @return Number of internal modules, 0 in case of invalid parameter.
 */
unsigned int ly_ctx_internal_modules_count(struct ly_ctx *ctx);

/**
 * @brief Add the search path into libyang context
 *
 * To reset search paths set in the context, use ly_ctx_unset_searchdirs() and then
 * set search paths again.
 *
 * @param[in] ctx Context to be modified.
 * @param[in] search_dir New search path to add to the current paths previously set in ctx.
 * @return EXIT_SUCCESS, EXIT_FAILURE.
 */
int ly_ctx_set_searchdir(struct ly_ctx *ctx, const char *search_dir);

/**
 * @brief Clean the search path(s) from the libyang context
 *
 * @param[in] ctx Context to be modified.
 * @param[in] index Index of the search path to be removed, use negative value to remove them all.
 *            Correct index value can be checked via ly_ctx_get_searchdirs().
 */
void ly_ctx_unset_searchdirs(struct ly_ctx *ctx, int index);

/**
 * @brief Get the NULL-terminated list of the search paths in libyang context.
 *
 * @param[in] ctx Context to query.
 * @return NULL-terminated list (array) of the search paths, NULL if no searchpath was set.
 */
const char * const *ly_ctx_get_searchdirs(const struct ly_ctx *ctx);

/**
 * @brief Get the currently set context's options.
 *
 * @param[in] ctx Context to query.
 * @return Combination of all the currently set context's options, see @ref contextoptions.
 */
int ly_ctx_get_options(struct ly_ctx *ctx);

/**
 * @brief Make context to stop searching for schemas (imported, included or requested via ly_ctx_load_module())
 * in searchdirs set via ly_ctx_set_searchdir() functions. Searchdirs are still stored in the context, so by
 * unsetting the option by ly_ctx_unset_disable_searchdirs() searching in all previously searchdirs is restored.
 *
 * The same effect is achieved by using #LY_CTX_DISABLE_SEARCHDIRS option when creating new context or parsing
 * a specific schema.
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_set_disable_searchdirs(struct ly_ctx *ctx);

/**
 * @brief Reverse function to ly_ctx_set_disable_searchdirs().
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_unset_disable_searchdirs(struct ly_ctx *ctx);

/**
 * @brief Make context to stop implicitly searching for schemas (imported, included or requested via ly_ctx_load_module())
 * in current working directory. This flag can be unset by ly_ctx_unset_disable_searchdir_cwd().
 *
 * The same effect is achieved by using #LY_CTX_DISABLE_SEARCHDIR_CWD option when creating new context or parsing
 * a specific schema.
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_set_disable_searchdir_cwd(struct ly_ctx *ctx);

/**
 * @brief Reverse function to ly_ctx_set_disable_searchdir_cwd().
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_unset_disable_searchdir_cwd(struct ly_ctx *ctx);

/**
 * @brief Prefer context's searchdirs before the user callback (ly_module_imp_clb) provided via ly_ctx_set_module_imp_clb()).
 *
 * The same effect is achieved by using #LY_CTX_PREFER_SEARCHDIRS option when creating new context or parsing
 * a specific schema.
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_set_prefer_searchdirs(struct ly_ctx *ctx);

/**
 * @brief Reverse function to ly_ctx_set_prefer_searchdirs().
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_unset_prefer_searchdirs(struct ly_ctx *ctx);

/**
 * @brief Make context to set all the imported modules to be implemented. By default,
 * if the imported module is not used in leafref's path, augment or deviation, it is
 * imported and its data tree is not taken into account.
 *
 * The same effect is achieved by using #LY_CTX_ALLIMPLEMENTED option when creating new context or parsing
 * a specific schema.
 *
 * Note, that function does not make the currently loaded modules, it just change the
 * schema parser behavior for the future parsing. This flag can be unset by ly_ctx_unset_allimplemented().
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_set_allimplemented(struct ly_ctx *ctx);

/**
 * @brief Reverse function to ly_ctx_set_allimplemented().
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_unset_allimplemented(struct ly_ctx *ctx);

/**
 * @brief Change the schema parser behavior when parsing new schemas forcing it to skip some of the schema
 * validation checks to improve performance. Note that parsing invalid schemas this way may lead to an
 * undefined behavior later, e.g. when working with data trees.
 *
 * The same effect is achieved by using #LY_CTX_TRUSTED option when creating new context or parsing
 * a specific schema.
 *
 * This flag can be unset by ly_ctx_unset_trusted().
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_set_trusted(struct ly_ctx *ctx);

/**
 * @brief Reverse function to ly_ctx_set_trusted().
 *
 * @param[in] ctx Context to be modified.
 */
void ly_ctx_unset_trusted(struct ly_ctx *ctx);

/**
 * @brief Get current ID of the modules set. The value is available also
 * as module-set-id in ly_ctx_info() result.
 *
 * @param[in] ctx Context to be examined.
 * @return Numeric identifier of the current context's modules set.
 */
uint16_t ly_ctx_get_module_set_id(const struct ly_ctx *ctx);

/**
 * @brief Get data of an internal ietf-yang-library module.
 *
 * @param[in] ctx Context with the modules.
 * @return Root data node corresponding to the model, NULL on error.
 * Caller is responsible for freeing the returned data tree using lyd_free().
 */
struct lyd_node *ly_ctx_info(struct ly_ctx *ctx);

/**
 * @brief Iterate over all (enabled) modules in a context.
 *
 * @param[in] ctx Context with the modules.
 * @param[in,out] idx Index of the next module to be returned. Value of 0 starts from the beginning.
 * @return Next context module, NULL if the last was already returned.
 */
const struct lys_module *ly_ctx_get_module_iter(const struct ly_ctx *ctx, uint32_t *idx);

/**
 * @brief Iterate over the disabled modules in a context.
 *
 * @param[in] ctx Context with the modules.
 * @param[in,out] idx Index of the next module to be returned. Value of 0 starts from the beginning.
 * @return Next context module, NULL if the last was already returned.
 */
const struct lys_module *ly_ctx_get_disabled_module_iter(const struct ly_ctx *ctx, uint32_t *idx);

/**
 * @brief Get pointer to the schema tree of the module of the specified name.
 *
 * @param[in] ctx Context to work in.
 * @param[in] name Name of the YANG module to get.
 * @param[in] revision Optional revision date of the YANG module to get. If not specified,
 * the schema in the newest/implemented revision (see \p implemented parameter) is returned if any.
 * @param[in] implemented In case the revision is not specified, require the implemented module
 * instead of the newest revision of the module.
 * @return Pointer to the data model structure, NULL if no schema following the name and
 * revision requirements is present in the context.
 */
const struct lys_module *ly_ctx_get_module(const struct ly_ctx *ctx, const char *name, const char *revision, int implemented);

/**
 * @brief Get pointer to the older schema tree to the specified one in the provided context.
 *
 * The module is not necessarily from the provided \p ctx. If there are multiple schemas older than the
 * provided one, the newest of them is returned.
 *
 * The function can be used in combination with ly_ctx_get_module() to get all revisions of a module in a context:
 * \code{.c}
 * for (mod = ly_ctx_get_module(ctx, name, NULL); mod; mod = ly_ctx_get_module_older(ctx, mod)) {
 *     ...
 * }
 * \endcode
 *
 * @param[in] ctx Context to work in.
 * @param[in] module YANG module to compare with
 * @return Pointer to the data model structure, NULL if no older schema is present in the context.
 */
const struct lys_module *ly_ctx_get_module_older(const struct ly_ctx *ctx, const struct lys_module *module);

/**
 * @brief Try to find the model in the searchpath of \p ctx and load it into it. If custom missing
 * module callback is set, it is used instead.
 *
 * If there is a possibility that the requested module is already in the context, you should call
 * the ly_ctx_get_module() first to avoid a lot of work performed by ly_ctx_load_module().
 *
 * @param[in] ctx Context to add to.
 * @param[in] name Name of the module to load.
 * @param[in] revision Optional revision date of the module. If not specified, it is
 * assumed that there is only one model revision in the searchpath (the first matching file
 * is parsed).
 * @return Pointer to the data model structure, NULL if not found or some error occurred.
 */
const struct lys_module *ly_ctx_load_module(struct ly_ctx *ctx, const char *name, const char *revision);

/**
 * @brief Callback for retrieving missing included or imported models in a custom way.
 *
 * @param[in] mod_name Missing module name.
 * @param[in] mod_rev Optional missing module revision.
 * @param[in] submod_name Optional missing submodule name.
 * @param[in] submod_rev Optional missing submodule revision.
 * @param[in] user_data User-supplied callback data.
 * @param[out] format Format of the returned module data.
 * @param[out] free_module_data Callback for freeing the returned module data. If not set, the data will be left untouched.
 * @return Requested module data, NULL if the module is supposed to be loaded
 * using standard mechanisms (searched for in the filesystem), NULL and #ly_errno set if
 * the callback failed resulting in the module failing to load.
 * If an @arg free_module_data callback is provided, it will be used to free the allegedly const data
 * which were returned by this callback.
 */
typedef const char *(*ly_module_imp_clb)(const char *mod_name, const char *mod_rev, const char *submod_name, const char *sub_rev,
                                   void *user_data, LYS_INFORMAT *format, void (**free_module_data)(void *model_data, void *user_data));

/**
 * @brief Set missing include or import module callback. It is meant to be used when the models
 * are not locally available (such as when downloading modules from a NETCONF server), it should
 * not be required in other cases.
 *
 * @param[in] ctx Context that will use this callback.
 * @param[in] clb Callback responsible for returning the missing model.
 * @param[in] user_data Arbitrary data that will always be passed to the callback \p clb.
 */
void ly_ctx_set_module_imp_clb(struct ly_ctx *ctx, ly_module_imp_clb clb, void *user_data);

/**
 * @brief Get the custom callback for missing import/include module retrieval.
 *
 * @param[in] ctx Context to read from.
 * @param[in] user_data Optional pointer for getting the user-supplied callback data.
 * @return Callback or NULL if not set.
 */
ly_module_imp_clb ly_ctx_get_module_imp_clb(const struct ly_ctx *ctx, void **user_data);

/**
 * @brief Callback for retrieving missing modules in the context, for which some data was found.
 *
 * Either \p name or \p ns is ALWAYS set, but both can also be set.
 *
 * @param[in,out] ctx Context with the missing module.
 * @param[in] name Missing module name.
 * @param[in] ns Missing module namespace.
 * @param[in] options Bitmask of LY_MODCLB_* values or 0.
 * @param[in] user_data User-supplied callback data.
 * @return Newly added or modified module, NULL on failure.
 */
typedef const struct lys_module *(*ly_module_data_clb)(struct ly_ctx *ctx, const char *name, const char *ns,
                                                       int options, void *user_data);

/* Module is in the context, but is not implemented, so to continue with the data operation, it must be implemented. */
#define LY_MODCLB_NOT_IMPLEMENTED 0x01

/**
 * @brief Set the missing data module callback. It will be called when some data is parsed or searched for and their module
 * is not found in the context or is not implemented.
 *
 * @param[in] ctx Context that will use this callback.
 * @param[in] clb Callback responsible for returning the missing model.
 * @param[in] user_data Arbitrary data that will always be passed to the callback \p clb.
 */
void ly_ctx_set_module_data_clb(struct ly_ctx *ctx, ly_module_data_clb clb, void *user_data);

/**
 * @brief Get the missing data module calback.
 *
 * @param[in] ctx Context to read from.
 * @param[in] user_data Optional pointer for getting the user-supplied callback data.
 * @return Callback or NULL if not set.
 */
ly_module_data_clb ly_ctx_get_module_data_clb(const struct ly_ctx *ctx, void **user_data);

#ifdef LY_ENABLED_LYD_PRIV

void ly_ctx_set_priv_dup_clb(struct ly_ctx *ctx, void *(*priv_dup_clb)(const void *priv));

#endif

/**
 * @brief Get pointer to the schema tree of the module of the specified namespace
 *
 * @param[in] ctx Context to work in.
 * @param[in] ns Namespace of the YANG module to get.
 * @param[in] revision Optional revision date of the YANG module to get. If not specified,
 * the schema in the newest/implemented revision (see \p implemented parameter) is returned if any.
 * @param[in] implemented In case the revision is not specified, require the implemented module
 * instead of the newest revision of the module.
 * @return Pointer to the data model structure, NULL if no schema following the namespace and
 * revision requirements is present in the context.
 */
const struct lys_module *ly_ctx_get_module_by_ns(const struct ly_ctx *ctx, const char *ns, const char *revision, int implemented);

/**
 * @brief Get submodule of a main module.
 *
 * If you already have the pointer to the submodule's main module, use ly_ctx_get_submodule2() instead.
 *
 * @param[in] ctx Context to work in.
 * @param[in] module Name of the main (belongs-to) module. If NULL, all module submodules are searched.
 * @param[in] revision Optional revision date of \p module. If NULL, all revisions of \p module
 * are searched. If set, \p module must also be set.
 * @param[in] submodule Name of the submodule to get.
 * @param[in] sub_revision Optional revision date of \p submodule. If NULL, the newest revision of \p submodule
 * is returned.
 * @return Pointer to the data model structure.
 */
const struct lys_submodule *ly_ctx_get_submodule(const struct ly_ctx *ctx, const char *module, const char *revision,
                                                 const char *submodule, const char *sub_revision);

/**
 * @brief Get submodule of a main module.
 *
 * If you have only the name (and optionally revision) of the submodule's main module, use ly_ctx_get_submodule()
 * instead.
 *
 * @param[in] main_module Main module (belongs to) of the searched submodule.
 * @param[in] submodule Name of the submodule to get.
 * @return Pointer to the data model structure.
 */
const struct lys_submodule *ly_ctx_get_submodule2(const struct lys_module *main_module, const char *submodule);

/**
 * @brief Get schema node according to the given data path (JSON format, see @ref howtoxpath).
 *
 * This function is meant as a utility function to ease getting a specific schema node
 * directly from context having \p data_path a const immutable string. Generally,
 * it is better to use lyd_find_path(), lys_find_path(), or ly_ctx_find_path(). Also,
 * it is possible to transform data paths into schema paths using ly_path_data2schema().
 *
 * @param[in] ctx Context to work in. Must be set if \p start is NULL.
 * @param[in] start Starting node for a relative data node identifier, in which
 * case it is mandatory and \p ctx can be NULL.
 * @param[in] data_path JSON data path of the node to get.
 * @param[in] output Search operation output instead input.
 * @return Resolved schema node or NULL.
 */
const struct lys_node *ly_ctx_get_node(const struct ly_ctx *ctx, const struct lys_node *start, const char *data_path, int output);

/**
 * @brief Get schema node according to the given schema path (see @ref howtoxpath).
 *
 * Note that the given path must be absolute and fully prefixed (which is the default output
 * of lys_path() function). To get node specified by a relative path, use lys_find_path() instead.
 *
 * @param[in] ctx Context to work in.
 * @param[in] path Schema path of the node to find.
 * @return Set of found schema nodes. If no nodes are matching \p path the returned set is empty.
 * In case of an error, NULL is returned.
 */
struct ly_set *ly_ctx_find_path(struct ly_ctx *ctx, const char *path);

/**
 * @brief Remove the specified module from its context.
 *
 * Beside the selected module, also all other modules depending on all the modules being removed
 * will be removed as well.
 *
 * libyang internal modules (those present when the context is created) cannot be removed.
 *
 * @param[in] module Module to be removed.
 * @param[in] private_destructor Optional destructor function for private objects assigned
 * to the nodes via lys_set_private(). If NULL, the private objects are not freed by libyang.
 * @return EXIT_SUCCESS or EXIT_FAILURE (in case of invalid parameter).
 */
int ly_ctx_remove_module(const struct lys_module *module,
                         void (*private_destructor)(const struct lys_node *node, void *priv));

/**
 * @brief Remove all the modules from the context except the internal modules. Also the addition data in
 * dictionary are kept.
 *
 * @param[in] ctx Context to work in.
 * @param[in] private_destructor Optional destructor function for private objects assigned
 * to the nodes via lys_set_private(). If NULL, the private objects are not freed by libyang.
 */
void ly_ctx_clean(struct ly_ctx *ctx, void (*private_destructor)(const struct lys_node *node, void *priv));

/**
 * @brief Free all internal structures of the specified context.
 *
 * The function should be used before terminating the application to destroy
 * and free all structures internally used by libyang. If the caller uses
 * multiple contexts, the function should be called for each used context.
 *
 * All instance data are supposed to be freed before destroying the context.
 * Data models are destroyed automatically as part of ly_ctx_destroy() call.
 *
 * @param[in] ctx libyang context to destroy
 * @param[in] private_destructor Optional destructor function for private objects assigned
 * to the nodes via lys_set_private(). If NULL, the private objects are not freed by libyang.
 * Remember the differences between the structures derived from ::lys_node and always check
 * ::lys_node#nodetype.
 */
void ly_ctx_destroy(struct ly_ctx *ctx, void (*private_destructor)(const struct lys_node *node, void *priv));

/**
 * @brief Transform a data path in XML format (node prefixes are XML namespace prefixes of module namespaces)
 * to JSON format (node prefixes are module names directly).
 *
 * @param[in] ctx Context to work in.
 * @param[in] xml_path Path in XML format to transform.
 * @param[in] xml XML tree to be used for XML namespace prefix resolution.
 * @return Transformed JSON data path, needs to be freed. NULL on error.
 */
char *ly_path_xml2json(struct ly_ctx *ctx, const char *xml_path, struct lyxml_elem *xml);

/**
 * @brief Transform a data path into schema path (see @ref howtoxpath).
 *
 * Some XPath tokens such as "*" or "//" should not be used because they cannot be properly
 * transformed into schema path. If used, any following nodes will not be transformed and simply copied,
 * so use at your own risk! It causes no problems if the path ends with the aforementioned tokens.
 *
 * @param[in] ctx Context to work in.
 * @param[in] data_path Data path to be transformed.
 * @return Created schema path, NULL on error.
 */
char *ly_path_data2schema(struct ly_ctx *ctx, const char *data_path);

/** @} context */

/**
 * @defgroup nodeset Tree nodes set
 * @ingroup datatree
 * @ingroup schematree
 * @{
 *
 * Structure and functions to hold and manipulate with sets of nodes from schema or data trees.
 */

/**
 * @brief set array of ::ly_set
 * It is kept in union to keep ::ly_set generic for data as well as schema trees
 */
union ly_set_set {
    struct lys_node **s;         /**< array of pointers to a ::lys_node objects */
    struct lyd_node **d;         /**< array of pointers to a ::lyd_node objects */
    void **g;                    /**< dummy array for generic work */
};

/**
 * @brief Structure to hold a set of (not necessary somehow connected) ::lyd_node or ::lys_node objects.
 * Caller is supposed to not mix the type of objects added to the set and according to its knowledge about
 * the set content, it is supposed to access the set via the sset, dset or set members of the structure.
 *
 * Until ly_set_rm() or ly_set_rm_index() is used, the set keeps the order of the inserted items as they
 * were added into the set, so the first added item is on array index 0.
 *
 * To free the structure, use ly_set_free() function, to manipulate with the structure, use other
 * ly_set_* functions.
 */
struct ly_set {
    unsigned int size;               /**< allocated size of the set array */
    unsigned int number;             /**< number of elements in (used size of) the set array */
    union ly_set_set set;            /**< set array - union to keep ::ly_set generic for data as well as schema trees */
};

/**
 * @brief Option for ly_set_add() to allow duplicities in the ly_set structure so the
 * set is not used as a set, but as a list of (container for) items.
 */
#define LY_SET_OPT_USEASLIST 0x01

/**
 * @brief Create and initiate new ::ly_set structure.
 *
 * @return Created ::ly_set structure or NULL in case of error.
 */
struct ly_set *ly_set_new(void);

/**
 * @brief Duplicate the existing set.
 *
 * @param[in] set Original set to duplicate
 * @return Duplication of the original set.
 */
struct ly_set *ly_set_dup(const struct ly_set *set);

/**
 * @brief Add a ::lyd_node or ::lys_node object into the set
 *
 * Since it is a set, the function checks for duplicity and if the
 * node is already in the set, the index of the previously added
 * node is returned.
 *
 * @param[in] set Set where the \p node will be added.
 * @param[in] node The ::lyd_node or ::lys_node object to be added into the \p set;
 * @param[in] options Options to change behavior of the function. Accepted options are:
 * - #LY_SET_OPT_USEASLIST - do not check for duplicities
 * @return -1 on failure, index of the \p node in the set on success
 */
int ly_set_add(struct ly_set *set, void *node, int options);

/**
 * @brief Add all objects from \p src to \p trg.
 *
 * Since it is a set, the function checks for duplicities.
 * After success, \p src is completely freed.
 *
 * @param[in] trg Target (result) set.
 * @param[in] src Source set.
 * @param[in] options Options to change behavior of the function. Accepted options are:
 * - #LY_SET_OPT_USEASLIST - do not check for duplicities
 * @return -1 on failure, number of objects added into \p trg on success.
 */
int ly_set_merge(struct ly_set *trg, struct ly_set *src, int options);

/**
 * @brief Get know if the set contains the specified object.
 * @param[in] set Set to explore.
 * @param[in] node Object to be found in the set.
 * @return Index of the object in the set or -1 if the object is not present in the set.
 */
int ly_set_contains(const struct ly_set *set, void *node);

/**
 * @brief Remove all objects from the set, but keep the set container for further use.
 *
 * @param[in] set Set to clean.
 * @return 0 on success
 */
int ly_set_clean(struct ly_set *set);

/**
 * @brief Remove a ::lyd_node or ::lys_node object from the set.
 *
 * Note that after removing a node from a set, indexes of other nodes in the set can change
 * (the last object is placed instead of the removed object).
 *
 * @param[in] set Set from which the \p node will be removed.
 * @param[in] node The ::lyd_node or ::lys_node object to be removed from the \p set;
 * @return 0 on success
 */
int ly_set_rm(struct ly_set *set, void *node);

/**
 * @brief Remove a ::lyd_node or ::lys_node object from the set index.
 *
 * Note that after removing a node from a set, indexes of other nodes in the set can change
 * (the last object is placed instead of the removed object).
 *
 * @param[in] set Set from which a node will be removed.
 * @param[in] index Index of the ::lyd_node or ::lys_node object in the \p set to be removed from the \p set;
 * @return 0 on success
 */
int ly_set_rm_index(struct ly_set *set, unsigned int index);

/**
 * @brief Free the ::ly_set data. Frees only the set structure content, not the referred data.
 *
 * @param[in] set The set to be freed.
 */
void ly_set_free(struct ly_set *set);

/**@} nodeset */

/**
 * @defgroup printerflags Printer flags
 * @ingroup datatree
 *
 * Validity flags for data nodes.
 *
 * @{
 */
#define LYP_WITHSIBLINGS  0x01  /**< Flag for printing also the (following) sibling nodes of the data node. */
#define LYP_FORMAT        0x02  /**< Flag for formatted output. */
#define LYP_KEEPEMPTYCONT 0x04  /**< Preserve empty non-presence containers */
#define LYP_WD_MASK       0xF0  /**< Mask for with-defaults modes */
#define LYP_WD_EXPLICIT   0x00  /**< Explicit mode - print only data explicitly being present in the data tree.
                                     Note that this is the default value when no WD option is specified. */
#define LYP_WD_TRIM       0x10  /**< Do not print the nodes with the value equal to their default value */
#define LYP_WD_ALL        0x20  /**< Include implicit default nodes */
#define LYP_WD_ALL_TAG    0x40  /**< Same as #LYP_WD_ALL but also adds attribute 'default' with value 'true' to
                                     all nodes that has its default value. The 'default' attribute has namespace:
                                     urn:ietf:params:xml:ns:netconf:default:1.0 and thus the attributes are
                                     printed only when the ietf-netconf-with-defaults module is present in libyang
                                     context (but in that case this namespace is always printed). */
#define LYP_WD_IMPL_TAG   0x80  /**< Same as LYP_WD_ALL_TAG but the attributes are added only to the nodes that
                                     are not explicitly present in the original data tree despite their
                                     value is equal to their default value.  There is the same limitation regarding
                                     the presence of ietf-netconf-with-defaults module in libyang context. */
#define LYP_NETCONF       0x100 /**< Print the data tree for use in NETCONF meaning:
                                     - for RPC output - skip the top-level RPC node,
                                     - for action output - skip all the parents of and the action node itself,
                                     - for action input - enclose the data in an action element in the base YANG namespace,
                                     - for all other data - print the whole data tree normally. */

/**
 * @}
 */

/**
 * @defgroup logger Logger
 * @{
 *
 * Publicly visible functions and values of the libyang logger. For more
 * information, see \ref howtologger.
 */

/**
 * @typedef LY_LOG_LEVEL
 * @brief Verbosity levels of the libyang logger.
 */
typedef enum {
    LY_LLERR = 0,     /**< Print only error messages, default value. */
    LY_LLWRN = 1,     /**< Print error and warning messages. */
    LY_LLVRB = 2,     /**< Besides errors and warnings, print some other verbose messages. */
    LY_LLDBG = 3      /**< Print all messages including some development debug messages (be careful,
                           without subsequently calling ly_verb_dbg() no debug messages will be printed!). */
} LY_LOG_LEVEL;

/**
 * @brief Set logger verbosity level.
 * @param[in] level Verbosity level.
 * @return Previous verbosity level.
 */
LY_LOG_LEVEL ly_verb(LY_LOG_LEVEL level);

/**
 * @defgroup logopts Logging options
 * @ingroup logger
 *
 * Logging option bits of libyang.
 *
 * @{
 */
#define LY_LOLOG        0x01 /**< Log messages normally, using callback if set. If not set, messages will
                                  not be printed by libyang. */
#define LY_LOSTORE      0x02 /**< Store any generated errors or warnings, never verbose or debug messages.
                                  Note that if #LY_LOLOG is not set then verbose and debug messages are always lost. */
#define LY_LOSTORE_LAST 0x06 /**< Store any generated errors or warnings but only the last message, always overwrite
                                  the previous one. */

/**
 * @}
 */

/**
 * @brief Set additional logger options. Default is #LY_LOLOG | #LY_LOSTORE_LAST.
 *
 * @param[in] opts Bitfield of @ref logopts.
 * @return Previous logger options.
 */
int ly_log_options(int opts);

#ifndef NDEBUG

/**
 * @defgroup dbggroup Debug message groups
 * @ingroup logger
 *
 * Selected displayed debug message groups.
 *
 * @{
 */

#define LY_LDGDICT  0x01 /**< Dictionary additions and deletions. */
#define LY_LDGYANG  0x02 /**< YANG parser messages. */
#define LY_LDGYIN   0x04 /**< YIN parser messages. */
#define LY_LDGXPATH 0x08 /**< XPath parsing end evaluation. */
#define LY_LDGDIFF  0x10 /**< Diff processing and creation. */
#define LY_LDGAPI   0x20 /**< API tracing. */
#define LY_LDGHASH  0x40 /**< Hash table modifications. */

/**
 * @}
 */

/**
 * @brief Enable specific debugging messages (independent of log level).
 * @param[in] dbg_groups Bitfield of enabled debug message groups (see @ref dbggroup).
 */
void ly_verb_dbg(int dbg_groups);

#endif

/**
 * @brief Set logger callback.
 *
 * !IMPORTANT! If an error has a specific error-app-tag defined in the model, it will NOT be set
 *             at the time of calling this callback. It will be set right after, so to retrieve it
 *             it must be checked afterwards with ly_errapptag().
 *
 * @param[in] clb Logging callback.
 * @param[in] path flag to resolve and provide path as the third parameter of the callback function. In case of
 *            validation and some other errors, it can be useful to get the path to the problematic element. Note,
 *            that according to the tree type and the specific situation, the path can slightly differs (keys
 *            presence) or it can be NULL, so consider it as an optional parameter. If the flag is 0, libyang will
 *            not bother with resolving the path.
 */
void ly_set_log_clb(void (*clb)(LY_LOG_LEVEL level, const char *msg, const char *path), int path);

/**
 * @brief Get logger callback.
 * @return Logger callback (can be NULL).
 */
void (*ly_get_log_clb(void))(LY_LOG_LEVEL, const char *, const char *);

/**
 * @typedef LY_ERR
 * @brief libyang's error codes available via ly_errno extern variable.
 * @ingroup logger
 */
typedef enum {
    LY_SUCCESS = 0, /**< no error, not set by functions, included just to complete #LY_ERR enumeration */
    LY_EMEM,        /**< Memory allocation failure */
    LY_ESYS,        /**< System call failure */
    LY_EINVAL,      /**< Invalid value */
    LY_EINT,        /**< Internal error */
    LY_EVALID,      /**< Validation failure */
    LY_EPLUGIN      /**< Error reported by a plugin */
} LY_ERR;

/**
 * @typedef LY_VECODE
 * @brief libyang's codes of validation error. Whenever ly_errno is set to LY_EVALID, the ly_vecode is also set
 * to the appropriate LY_VECODE value.
 * @ingroup logger
 */
typedef enum {
    LYVE_SUCCESS = 0,  /**< no error */

    LYVE_XML_MISS,     /**< missing XML object */
    LYVE_XML_INVAL,    /**< invalid XML object */
    LYVE_XML_INCHAR,   /**< invalid XML character */

    LYVE_EOF,          /**< unexpected end of input data */
    LYVE_INSTMT,       /**< invalid statement (schema) */
    /* */
    LYVE_INPAR,        /**< invalid (in)direct parent (schema) */
    LYVE_INID,         /**< invalid identifier (schema) */
    LYVE_INDATE,       /**< invalid date format */
    LYVE_INARG,        /**< invalid value of a statement argument (schema) */
    LYVE_MISSSTMT,     /**< missing required statement (schema) */
    /* */
    LYVE_MISSARG,      /**< missing required statement argument (schema) */
    LYVE_TOOMANY,      /**< too many instances of some object */
    LYVE_DUPID,        /**< duplicated identifier (schema) */
    LYVE_DUPLEAFLIST,  /**< multiple instances of leaf-list */
    LYVE_DUPLIST,      /**< multiple instances of list */
    LYVE_NOUNIQ,       /**< unique leaves match on 2 list instances (data) */
    LYVE_ENUM_INVAL,   /**< invalid enum value (schema) */
    LYVE_ENUM_INNAME,  /**< invalid enum name (schema) */
    /* */
    /* */
    LYVE_ENUM_WS,      /**< enum name with leading/trailing whitespaces (schema) */
    LYVE_BITS_INVAL,   /**< invalid bits value (schema) */
    LYVE_BITS_INNAME,  /**< invalid bits name (schema) */
    /* */
    /* */
    LYVE_INMOD,        /**< invalid module name */
    /* */
    LYVE_KEY_NLEAF,    /**< list key is not a leaf (schema) */
    LYVE_KEY_TYPE,     /**< invalid list key type (schema) */
    LYVE_KEY_CONFIG,   /**< key config value differs from the list config value */
    LYVE_KEY_MISS,     /**< list key not found (schema) */
    LYVE_KEY_DUP,      /**< duplicated key identifier (schema) */
    LYVE_INREGEX,      /**< invalid regular expression (schema) */
    LYVE_INRESOLV,     /**< no resolvents found (schema) */
    LYVE_INSTATUS,     /**< invalid derivation because of status (schema) */
    LYVE_CIRC_LEAFREFS,/**< circular chain of leafrefs detected (schema) */
    LYVE_CIRC_FEATURES,/**< circular chain of features detected (schema) */
    LYVE_CIRC_IMPORTS, /**< circular chain of imports detected (schema) */
    LYVE_CIRC_INCLUDES,/**< circular chain of includes detected (schema) */
    LYVE_INVER,        /**< non-matching YANG versions of module and its submodules (schema) */
    LYVE_SUBMODULE,    /**< submodule given instead of a module */

    LYVE_OBSDATA,      /**< obsolete data instantiation (data) */
    /* */
    LYVE_NORESOLV,     /**< no resolvents found for an expression (data) */
    LYVE_INELEM,       /**< invalid element (data) */
    /* */
    LYVE_MISSELEM,     /**< missing required element (data) */
    LYVE_INVAL,        /**< invalid value of an element (data) */
    LYVE_INMETA,       /**< invalid metadata (attribute) value (data) */
    LYVE_INATTR,       /**< invalid attribute in an element (data) */
    LYVE_MISSATTR,     /**< missing attribute in an element (data) */
    LYVE_NOCONSTR,     /**< value out of range/length/pattern (data) */
    LYVE_INCHAR,       /**< unexpected characters (data) */
    LYVE_INPRED,       /**< predicate resolution fail (data) */
    LYVE_MCASEDATA,    /**< data for more cases of a choice (data) */
    LYVE_NOMUST,       /**< unsatisfied must condition (data) */
    LYVE_NOWHEN,       /**< unsatisfied when condition (data) */
    LYVE_INORDER,      /**< invalid order of elements (data) */
    LYVE_INWHEN,       /**< irresolvable when condition (data) */
    LYVE_NOMIN,        /**< min-elements constraint not honored (data) */
    LYVE_NOMAX,        /**< max-elements constraint not honored (data) */
    LYVE_NOREQINS,     /**< required instance does not exits (data) */
    LYVE_NOLEAFREF,    /**< leaf pointed to by leafref does not exist (data) */
    LYVE_NOMANDCHOICE, /**< no mandatory choice case branch exists (data) */

    LYVE_XPATH_INTOK,  /**< unexpected XPath token */
    LYVE_XPATH_EOF,    /**< unexpected end of an XPath expression */
    LYVE_XPATH_INOP,   /**< invalid XPath operation operands */
    /* */
    LYVE_XPATH_INCTX,  /**< invalid XPath context type */
    LYVE_XPATH_INMOD,  /**< invalid module name */
    LYVE_XPATH_INFUNC, /**< invalid XPath function name */
    LYVE_XPATH_INARGCOUNT, /**< invalid number of arguments for an XPath function */
    LYVE_XPATH_INARGTYPE, /**< invalid type of arguments for an XPath function */
    LYVE_XPATH_DUMMY,  /**< invalid use of the XPath dummy node */
    LYVE_XPATH_NOEND,  /**< unterminated string */

    LYVE_PATH_INCHAR,  /**< invalid characters (path) */
    LYVE_PATH_INMOD,   /**< invalid module name (path) */
    LYVE_PATH_MISSMOD, /**< missing module name (path) */
    LYVE_PATH_INNODE,  /**< invalid node name (path) */
    LYVE_PATH_INKEY,   /**< invalid key name (path) */
    LYVE_PATH_MISSKEY, /**< missing some list keys (path) */
    LYVE_PATH_INIDENTREF, /**< missing module name prefix in identityref predicate value (path) */
    LYVE_PATH_EXISTS,  /**< target node already exists (path) */
    LYVE_PATH_MISSPAR, /**< some parent of the target node is missing (path) */
    LYVE_PATH_PREDTOOMANY, /**< too many predicates specified (path) */
} LY_VECODE;

/**
 * @cond INTERNAL
 * Get address of (thread-specific) `ly_errno' variable.
 */
LY_ERR *ly_errno_glob_address(void);

/**
 * @endcond INTERNAL
 * @brief libyang specific (thread-safe) errno (see #LY_ERR for the list of possible values and their meaning).
 */
#define ly_errno (*ly_errno_glob_address())

/**
 * @brief Get the last (thread, context-specific) validation error code.
 *
 * This value is set only if ly_errno is #LY_EVALID.
 *
 * @param[in] ctx Relative context.
 * @return Validation error code.
 */
LY_VECODE ly_vecode(const struct ly_ctx *ctx);

/**
 * @brief Get the last (thread, context-specific) error message. If the corresponding module defined
 * a specific error message, it will be used instead the default one.
 *
 * Sometimes, the error message is extended with path of the element where the problem is.
 * The path is available via ly_errpath().
 *
 * @param[in] ctx Relative context.
 * @return Text of the last error message, empty string if there is no error.
 */
const char *ly_errmsg(const struct ly_ctx *ctx);

/**
 * @brief Get the last (thread, context-specific) path of the element where was an error.
 *
 * The path always corresponds to the error message available via ly_errmsg(), so
 * whenever a subsequent error message is printed, the path is erased or rewritten.
 * The path reflects the type of the processed tree - data path for data tree functions
 * and schema path in case of schema tree functions. In case of processing YIN schema
 * or XML data, the path can be just XML path. In such a case, the corresponding
 * ly_vecode (value 1-3) is set.
 *
 * @param[in] ctx Relative context.
 * @return Path of the error element, empty string if error path does not apply to the last error.
 */
const char *ly_errpath(const struct ly_ctx *ctx);

/**
 * @brief Get the last (thread, context-specific) error-app-tag if there was a specific one defined
 * in the module for the last error.
 *
 * The app-tag always corresponds to the error message available via ly_errmsg(), so
 * whenever a subsequent error message is printed, the app-tag is erased or rewritten.
 *
 * @param[in] ctx Relative context.
 * @return Error-app-tag of the last error, empty string if the error-app-tag does not apply to the last error.
 */
const char *ly_errapptag(const struct ly_ctx *ctx);

/**
 * @brief Libyang full error structure.
 */
struct ly_err_item {
    LY_LOG_LEVEL level;
    LY_ERR no;
    LY_VECODE vecode;
    char *msg;
    char *path;
    char *apptag;
    struct ly_err_item *next;
    struct ly_err_item *prev; /* first item's prev points to the last item */
};

/**
 * @brief Get the first (thread, context-specific) generated error structure.
 *
 * @param[in] ctx Relative context.
 * @return First error structure (can be NULL), do not modify!
 */
struct ly_err_item *ly_err_first(const struct ly_ctx *ctx);

/**
 * @brief Print the error structure as if just generated.
 *
 * @param[in] eitem Error item structure to print.
 */
void ly_err_print(struct ly_err_item *eitem);

/**
 * @brief Free error structures from a context.
 *
 * If \p eitem is not set, free all the error structures.
 *
 * @param[in] ctx Relative context.
 * @param[in] eitem Oldest error structure to remove, optional.
 */
void ly_err_clean(struct ly_ctx *ctx, struct ly_err_item *eitem);

/**
 * @} logger
 */

#ifdef __cplusplus
}
#endif

#endif /* LY_LIBYANG_H_ */
