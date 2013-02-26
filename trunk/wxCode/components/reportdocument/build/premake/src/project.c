/**********************************************************************
 * Premake - project.c
 * An interface around the project data.
 *
 * Copyright (c) 2002-2005 Jason Perkins and the Premake project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License in the file LICENSE.txt for details.
 **********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "premake.h"
#include "os.h"

Project* project = NULL;

static Package*    my_pkg  = NULL;
static PkgConfig*  my_cfg  = NULL;
static FileConfig* my_fcfg = NULL;
static Option*     my_opt  = NULL;

static char buffer[8192];


/************************************************************************
 * Project lifecycle routines
 ***********************************************************************/

void   prj_open()
{
	if (project != NULL)
		prj_close();
	project = ALLOCT(Project);
}


void   prj_close()
{
	int i, j;

	if (project != NULL)
	{
		for (i = 0; i < prj_get_numpackages(); ++i)
		{
			Package* package = project->packages[i];

			for (j = 0; j < prj_get_numconfigs(); ++j)
			{
				PkgConfig* config = package->configs[j];
				free((void*)config->buildopts);
				free((void*)config->defines);
				free((void*)config->files);
				free((void*)config->flags);
				free((void*)config->incpaths);
				free((void*)config->libpaths);
				free((void*)config->linkopts);
				free((void*)config->links);
				prj_freelist((void**)config->fileconfigs);
			}

			prj_freelist((void**)package->configs);
			if (package->data != NULL)
				free(package->data);
		}

		prj_freelist((void**)project->options);
		prj_freelist((void**)project->configs);
		prj_freelist((void**)project->packages);
		free(project);
		project = NULL;
	}
}


/************************************************************************
 * Locate a package by name
 ***********************************************************************/

int prj_find_package(const char* name)
{
	int i;
	for (i = 0; i < prj_get_numpackages(); ++i)
	{
		if (matches(name, project->packages[i]->name))
			return i;
	}
	return -1;
}


/************************************************************************
 * Retrieve the active configuration for the given package
 ***********************************************************************/

PkgConfig* prj_get_config_for(int i)
{
	int j;
	for (j = 0; j < prj_get_numconfigs(); ++j)
	{
		if (my_pkg->configs[j] == my_cfg)
			return project->packages[i]->configs[j];
	}
	return NULL;
}


/************************************************************************
 * Get/set a file build action
 ***********************************************************************/

const char* prj_get_buildaction()
{
	return my_fcfg->buildaction;
}

int prj_is_buildaction(const char* action)
{
	return matches(my_fcfg->buildaction, action);
}

void prj_set_buildaction(const char* action)
{
	my_fcfg->buildaction = action;
}


/************************************************************************
 * Get/set generator-specific data
 ***********************************************************************/

void* prj_get_data()
{
	return prj_get_data_for(my_pkg->index);
}

void* prj_get_data_for(int i)
{
	return project->packages[i]->data;
}

void prj_set_data(void* data)
{
	my_pkg->data = data;
}


/************************************************************************
 * Return the list of defines for the current object
 ***********************************************************************/

int prj_get_numdefines()
{
	return prj_getlistsize((void**)prj_get_defines());
}

const char** prj_get_defines()
{
	return my_cfg->defines;
}


/************************************************************************
 * Query the object directories
 ***********************************************************************/

const char* prj_get_bindir()
{
	if (my_cfg->bindir != NULL)
	{
		return my_cfg->bindir;
	}
	else
	{
		strcpy(buffer, path_build(my_pkg->path, my_cfg->prjConfig->bindir));
		return buffer;
	}
}

/*
 * Get the relative directory FROM the current package TO the
 * binary output directory of the specified package.
 */
const char* prj_get_bindir_for(int i)
{
	PkgConfig* cfg = prj_get_config_for(i);
	if (cfg->bindir != NULL)
	{
		const char* target;

		Package* pkg = prj_get_package_for(i);
		strcpy(buffer, path_build(my_pkg->path, pkg->path));
		strcat(buffer, "/");
		strcat(buffer, cfg->bindir);

		/* Now convert that to a relative path from here */
		target = path_build("", buffer);
		if (target[0] == '/') ++target;
		return target;
	}
	else
	{
//		strcpy(buffer, path_build(my_pkg->path, prj_get_path()));
//		strcpy(buffer, "/");
//		strcpy(buffer, cfg->prjConfig->bindir);
		strcpy(buffer, path_build(my_pkg->path, my_cfg->prjConfig->bindir));
		return buffer;
	}
}

const char* prj_get_libdir()
{
	if (my_cfg->libdir != NULL)
	{
		return my_cfg->libdir;
	}
	else
	{
		strcpy(buffer, path_build(my_pkg->path, my_cfg->prjConfig->libdir));
		return buffer;
	}
}

const char* prj_get_libdir_for(int i)
{
	PkgConfig* cfg = prj_get_config_for(i);
	if (cfg->libdir != NULL)
	{
		const char* target;

		Package* pkg = prj_get_package_for(i);
		strcpy(buffer, path_build(my_pkg->path, pkg->path));
		strcat(buffer, "/");
		strcat(buffer, cfg->libdir);

		/* Now convert that to a relative path from here */
		target = path_build("", buffer);
		if (target[0] == '/') ++target;
		return target;
	}
	else
	{
		strcpy(buffer, path_build(my_pkg->path, my_cfg->prjConfig->libdir));
		return buffer;
	}
}

const char* prj_get_objdir()
{
	if (my_cfg->objdir != NULL)
	{
		return my_cfg->objdir;
	}
	else
	{
		return path_combine(my_pkg->objdir, my_cfg->prjConfig->name);
	}
}

const char* prj_get_pkgobjdir()
{
	if (my_cfg->objdir == NULL)
	{
		return my_pkg->objdir;
	}
	else
	{
		return NULL;
	}
}

const char* prj_get_outdir()
{
	return prj_get_outdir_for(my_pkg->index);
}

const char* prj_get_outdir_for(int i)
{
	const char* targetdir;
	PkgConfig* cfg = prj_get_config_for(i);

	if (matches(cfg->kind, "lib"))
		strcpy(buffer, prj_get_libdir_for(i));
	else
		strcpy(buffer, prj_get_bindir_for(i));

	targetdir = path_getdir(cfg->target);
	if (strlen(targetdir) > 0)
	{
		strcat(buffer, "/");
		strcat(buffer, targetdir);
	}

	return buffer;
}


/************************************************************************
 * Custom target decorations
 ***********************************************************************/

const char* prj_get_prefix()
{
	return my_cfg->prefix;
}

const char* prj_get_extension()
{
	return my_cfg->extension;
}


/************************************************************************
 * Return the files associated with the active object
 ***********************************************************************/

const char** prj_get_files()
{
	return my_cfg->files;
}

int prj_has_file(const char* name)
{
	const char** ptr = my_cfg->files;
	while (*ptr != NULL)
	{
		if (matches(*ptr, name))
			return 1;
		ptr++;
	}

	return 0;
}

const char* prj_find_filetype(const char* extension)
{
	const char** ptr = my_cfg->files;
	while (*ptr != NULL)
	{
		if (matches(path_getextension(*ptr), extension))
			return *ptr;
		ptr++;
	}

	return NULL;
}


/************************************************************************
 * Query the build flags
 ***********************************************************************/

int prj_has_flag(const char* flag)
{
	return prj_has_flag_for(my_pkg->index, flag);
}

int prj_has_flag_for(int i, const char* flag)
{
	PkgConfig* cfg = prj_get_config_for(i);
	const char** ptr = cfg->flags;
	while (*ptr != NULL)
	{
		if (matches(*ptr, flag))
			return 1;
		ptr++;
	}

	return 0;
}

int prj_get_numbuildoptions()
{
	return prj_getlistsize((void**)prj_get_buildoptions());
}

const char** prj_get_buildoptions()
{
	return my_cfg->buildopts;
}

int prj_get_numlinkoptions()
{
	return prj_getlistsize((void**)prj_get_linkoptions());
}

const char** prj_get_linkoptions()
{
	return my_cfg->linkopts;
}


/************************************************************************
 * Return the list of pre/post build commands for the current object
 ***********************************************************************/

int prj_get_numprebuildcommands()
{
	return prj_getlistsize((void**)prj_get_prebuildcommands());
}

const char** prj_get_prebuildcommands()
{
	return my_cfg->prebuildcmds;
}

int prj_get_numprelinkcommands()
{
	return prj_getlistsize((void**)prj_get_prelinkcommands());
}

const char** prj_get_prelinkcommands()
{
	return my_cfg->prelinkcmds;
}

int prj_get_numpostbuildcommands()
{
	return prj_getlistsize((void**)prj_get_postbuildcommands());
}

const char** prj_get_postbuildcommands()
{
	return my_cfg->postbuildcmds;
}


/************************************************************************
 * Query the target kind of the current object
 ***********************************************************************/

const char* prj_get_kind()
{
	return my_cfg->kind;
}

int prj_is_kind(const char* kind)
{
	return matches(my_cfg->kind, kind);
}

const char* prj_get_kind_for(int i)
{
	PkgConfig* cfg = prj_get_config_for(i);
	return cfg->kind;
}


/************************************************************************
 * Return the list of include paths for the current object
 ***********************************************************************/

int prj_get_numincpaths()
{
	return prj_getlistsize((void**)prj_get_incpaths());
}

const char** prj_get_incpaths()
{
	return my_cfg->incpaths;
}


/************************************************************************
 * Query the language of the current object
 ***********************************************************************/

const char* prj_get_language()
{
	return my_pkg->lang;
}

const char* prj_get_language_for(int i)
{
	return project->packages[i]->lang;
}

int prj_is_lang(const char* lang)
{
	return matches(my_pkg->lang, lang);
}


/************************************************************************
 * Return the list of linker paths for the current object
 ***********************************************************************/

const char** prj_get_libpaths()
{
	return my_cfg->libpaths;
}


/************************************************************************
 * Return the list of links for the current object
 ***********************************************************************/

int prj_get_numlinks()
{
	return prj_getlistsize((void**)prj_get_links());
}

const char** prj_get_links()
{
	return my_cfg->links;
}


/************************************************************************
 * Return the name of the active object
 ***********************************************************************/

const char* prj_get_name()
{
	return project->name;
}

const char* prj_get_cfgname()
{
	return my_cfg->prjConfig->name;
}

const char* prj_get_pkgname()
{
	return my_pkg->name;
}

const char* prj_get_pkgname_for(int i)
{
	return project->packages[i]->name;
}


/************************************************************************
 * Return the total number of configurations in the project
 ***********************************************************************/

int prj_get_numconfigs()
{
	return prj_getlistsize((void**)project->configs);
}


/************************************************************************
 * Return the total number of options in the project
 ***********************************************************************/

int prj_get_numoptions()
{
	return prj_getlistsize((void**)project->options);
}


/************************************************************************
 * Return the total number of packages in the project
 ***********************************************************************/

int prj_get_numpackages()
{
	return prj_getlistsize((void**)project->packages);
}



/************************************************************************
 * Return the name and description of the currently selected option.
 ***********************************************************************/

const char* prj_get_optdesc()
{
	return my_opt->desc;
}

const char* prj_get_optname()
{
	return my_opt->flag;
}


/************************************************************************
 * Return the active object
 ***********************************************************************/

Package* prj_get_package()
{
	return my_pkg;
}

Package* prj_get_package_for(int i)
{
	return project->packages[i];
}


/************************************************************************
 * Return the path to the generated object scripts.
 ***********************************************************************/

const char* prj_get_path()
{
	return project->path;
}

const char* prj_get_pkgpath()
{
	return my_pkg->path;
}

const char* prj_get_pkgpath_for(int i)
{
	return project->packages[i]->path;
}

const char* prj_get_pkgfilename(const char* extension)
{
	return prj_get_pkgfilename_for(my_pkg->index, extension);
}

const char* prj_get_pkgfilename_for(int i, const char* extension)
{
	strcpy(buffer, path_build(project->path, prj_get_pkgpath_for(i)));
	if (strlen(buffer) > 0)
		strcat(buffer, "/");
	strcat(buffer, project->packages[i]->name);
	if (extension != NULL)
	{
		strcat(buffer, ".");
		strcat(buffer, extension);
	}
	return buffer;
}


/************************************************************************
 * Get the PCH information for the configuration.
 ***********************************************************************/

int prj_has_pch()
{
	return (!prj_has_flag("no-pch") && my_cfg->pchHeader != NULL);
}

const char* prj_get_pch_header()
{
	return my_cfg->pchHeader;
}

const char* prj_get_pch_source()
{
	return my_cfg->pchSource;
}


/************************************************************************
 * Query the list of resource defines for the current object
 ***********************************************************************/

int prj_get_numresdefines()
{
	return prj_getlistsize((void**)prj_get_resdefines());
}

const char** prj_get_resdefines()
{
	return my_cfg->resdefines;
}


/************************************************************************
 * Query the list of resource options for the current object
 ***********************************************************************/

int prj_get_numresoptions()
{
	return prj_getlistsize((void**)prj_get_resoptions());
}
 
const char** prj_get_resoptions()
{
	return my_cfg->resopts;
}


/************************************************************************
 * Query the list of resource paths for the current object
 ***********************************************************************/

int prj_get_numrespaths()
{
	return prj_getlistsize((void**)prj_get_respaths());
}

const char** prj_get_respaths()
{
	return my_cfg->respaths;
}


/************************************************************************
 * Return the script name for an object.
 ***********************************************************************/

const char* prj_get_script()
{
	return project->script;
}

const char* prj_get_pkgscript()
{
	return my_pkg->script;
}


/************************************************************************
 * Return the target for the active object. There are a couple of 
 * variants here; at some point I need to walk through all the usages
 * and see if I can merge these into one method.
 ***********************************************************************/

const char* prj_get_target()
{
	return prj_get_target_for(my_pkg->index);
}

const char* prj_get_target_for(int i)
{
	const char* extension = "";

	/* Get the active configuration for this target */
	Package* pkg = prj_get_package_for(i);
	PkgConfig* cfg = prj_get_config_for(i);
	const char* filename = path_getbasename(cfg->target);

	/* Prepopulate the buffer with the output directory */
	prj_get_outdir_for(i);
	if (matches(buffer, "."))
		strcpy(buffer, "");
	if (strlen(buffer) > 0)
		strcat(buffer, "/");

	if (cfg->prefix != NULL)
		strcat(buffer, cfg->prefix);

	if (matches(pkg->lang, "c#"))
	{
		strcat(buffer, filename);
		if (matches(cfg->kind, "dll"))
			extension = "dll";
		else
			extension = "exe";
	}

	else if (os_is("windows"))
	{
		/* CodeBlocks and GNU uses Unix naming for static libs */
		if ((matches(g_target, "cb-gcc") || matches(g_target, "cl-gcc") || matches(g_target, "gnu")) && 
			matches(cfg->kind, "lib"))
		{
			if (cfg->prefix == NULL)
				strcat(buffer, "lib");
			strcat(buffer, filename);
			extension = "a";
		}
		else
		{
			strcat(buffer, filename);
			if (matches(cfg->kind, "lib"))
				extension = "lib";
			else if (matches(cfg->kind, "dll"))
				extension = "dll";
			else
				extension = "exe";
		}
	}

	else if (os_is("macosx") && matches(cfg->kind, "dll"))
	{
		if (cfg->prefix == NULL)
			strcat(buffer, "lib");
		strcat(buffer, filename);
		if (prj_has_flag_for(i, "dylib"))
			extension = "dylib";
		else
			extension = "so";
	}

	else
	{
		if (matches(cfg->kind, "lib"))
		{
			if (cfg->prefix == NULL)
				strcat(buffer, "lib");
			strcat(buffer, filename);
			extension = "a";
		}
		else if (matches(cfg->kind, "dll"))
		{
			if (cfg->prefix == NULL)
				strcat(buffer, "lib");
			strcat(buffer, filename);
			extension = "so";
		}
		else
		{
			strcat(buffer, filename);
		}
	}

	/* Apply the file extension, which can be customized */
	if (cfg->extension != NULL)
		extension = cfg->extension;

	if (strlen(extension) > 0)
		strcat(buffer, ".");

	strcat(buffer, extension);

	return buffer;
}

/*
 * Okay - I need to start documenting these functions better; I
 * can't remember what does what any more. This functions gets
 * the path FROM the current package directory TO the target of
 * the specified package. As you can see, I got off on the wrong
 * track here, and I'm trying to get it back.
 */
const char* prj_get_relativetarget_for(int i)
{
	return prj_get_target_for(i);

#if OBSOLETE
	char backpath[8192];
	const char* target;

	/* Get the path back to the main project so I have a common
	 * point of reference between myself and this dependency */
	strcpy(backpath, path_build(prj_get_pkgpath(), prj_get_path()));

	/* Append the path to the dependent project library */
	if (strlen(backpath) > 0)
		strcat(backpath, "/");
	strcat(backpath, prj_get_pkgpath_for(i));

	/* Append the path from the dependent project to its output dir */
	if (strlen(backpath) > 0)
		strcat(backpath, "/");
	strcat(backpath, prj_get_target_for(i));

	/* Now convert that to a relative path from here */
	target = path_build("", backpath);
	if (target[0] == '/') ++target;
	return target;
#endif
}

const char* prj_get_target_raw()
{
	return my_cfg->target;
}

const char* prj_get_targetname_for(int i)
{
	PkgConfig* cfg = prj_get_config_for(i);
	strcpy(buffer, path_getname(cfg->target));
	return buffer;
}


/************************************************************************
 * Return package URL
 ***********************************************************************/

const char* prj_get_url()
{
	return my_pkg->url;
}


/************************************************************************
 * Activate a project object.
 ***********************************************************************/

void prj_select_config(int i)
{
	if (my_pkg == NULL)
		prj_select_package(0);
	my_cfg = my_pkg->configs[i];
}

void prj_select_file(const char* name)
{
	int i;
	for (i = 0; i < prj_getlistsize((void**)my_cfg->files); ++i)
	{
		if (matches(my_cfg->files[i], name))
			my_fcfg = my_cfg->fileconfigs[i];
	}
}

void prj_select_option(int i)
{
	my_opt = project->options[i];
}

void prj_select_package(int i)
{
	my_pkg = project->packages[i];
}


/************************************************************************
 * List management routines
 ***********************************************************************/

void** prj_newlist(int len)
{
	void** list = (void**)malloc(sizeof(void*) * (len + 1));
	list[len] = NULL;
	return list;
}


void prj_freelist(void** list)
{
	int i = 0;
	while (list[i] != NULL)
		free(list[i++]);
	free(list);
}


int prj_getlistsize(void** list)
{
	int count = 0;
	void** ptr = list;
	while (*ptr != NULL)
	{
		ptr++;
		count++;
	}
	return count;
}


int prj_has_guid()
{
	return (my_pkg->guid != NULL);
}


const char* prj_get_guid()
{
	return my_pkg->guid;
}


int prj_has_libpath(const char* path)
{
	const char** libpath;
	
	if (path == NULL || strlen(path) == 0)
		return 1;

	libpath = prj_get_libpaths();
	while (*libpath)
	{
		if (matches(*libpath, path))
			return 1;
		libpath++;
	}

	if (matches(prj_get_libdir(), path))
		return 1;

	if (matches(prj_get_bindir(), path))
		return 1;

	return 0;
}


int prj_has_importlibname()
{
	return (my_cfg->implibname != NULL);
}

const char* prj_get_importlibname()
{
	return my_cfg->implibname;
}
