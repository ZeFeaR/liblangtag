/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/* 
 * lt-region.c
 * Copyright (C) 2011-2012 Akira TAGOH
 * 
 * Authors:
 *   Akira TAGOH  <akira@tagoh.org>
 * 
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "lt-mem.h"
#include "lt-region.h"
#include "lt-region-private.h"


struct _lt_region_t {
	lt_mem_t  parent;
	gchar    *tag;
	gchar    *description;
	gchar    *preferred_tag;
};


/*< private >*/

/*< protected >*/
lt_region_t *
lt_region_create(void)
{
	lt_region_t *retval = lt_mem_alloc_object(sizeof (lt_region_t));

	return retval;
}

void
lt_region_set_name(lt_region_t *region,
		   const gchar *description)
{
	g_return_if_fail (region != NULL);
	g_return_if_fail (description != NULL);

	if (region->description)
		lt_mem_remove_ref(&region->parent, region->description);
	region->description = g_strdup(description);
	lt_mem_add_ref(&region->parent, region->description,
		       (lt_destroy_func_t)g_free);
}

void
lt_region_set_tag(lt_region_t *region,
		  const gchar *subtag)
{
	g_return_if_fail (region != NULL);
	g_return_if_fail (subtag != NULL);

	if (region->tag)
		lt_mem_remove_ref(&region->parent, region->tag);
	region->tag = g_strdup(subtag);
	lt_mem_add_ref(&region->parent, region->tag,
		       (lt_destroy_func_t)g_free);
}

void
lt_region_set_preferred_tag(lt_region_t *region,
			    const gchar *subtag)
{
	g_return_if_fail (region != NULL);
	g_return_if_fail (subtag != NULL);

	if (region->preferred_tag)
		lt_mem_remove_ref(&region->parent, region->preferred_tag);
	region->preferred_tag = g_strdup(subtag);
	lt_mem_add_ref(&region->parent, region->preferred_tag,
		       (lt_destroy_func_t)g_free);
}

/*< public >*/
lt_region_t *
lt_region_ref(lt_region_t *region)
{
	g_return_val_if_fail (region != NULL, NULL);

	return lt_mem_ref(&region->parent);
}

void
lt_region_unref(lt_region_t *region)
{
	if (region)
		lt_mem_unref(&region->parent);
}

const gchar *
lt_region_get_name(const lt_region_t *region)
{
	g_return_val_if_fail (region != NULL, NULL);

	return region->description;
}

const gchar *
lt_region_get_better_tag(const lt_region_t *region)
{
	const gchar *retval = lt_region_get_preferred_tag(region);

	if (!retval)
		retval = lt_region_get_tag(region);

	return retval;
}

const gchar *
lt_region_get_tag(const lt_region_t *region)
{
	g_return_val_if_fail (region != NULL, NULL);

	return region->tag;
}

const gchar *
lt_region_get_preferred_tag(const lt_region_t *region)
{
	g_return_val_if_fail (region != NULL, NULL);

	return region->preferred_tag;
}

void
lt_region_dump(const lt_region_t *region)
{
	GString *string = g_string_new(NULL);
	const gchar *preferred = lt_region_get_preferred_tag(region);

	if (preferred) {
		if (string->len == 0)
			g_string_append(string, " (");
		g_string_append_printf(string, "preferred-value: %s", preferred);
	}
	if (string->len > 0)
		g_string_append(string, ")");

	g_print("Region: %s [%s]%s\n",
		lt_region_get_tag(region),
		lt_region_get_name(region),
		string->str);
	g_string_free(string, TRUE);
}

gboolean
lt_region_compare(const lt_region_t *v1,
		  const lt_region_t *v2)
{
	g_return_val_if_fail (v1 != NULL, FALSE);
	g_return_val_if_fail (v2 != NULL, FALSE);

	if (v1 == v2)
		return TRUE;

	return g_strcmp0(lt_region_get_tag(v1), lt_region_get_tag(v2)) == 0;
}
