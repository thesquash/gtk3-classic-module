/* Treeview Zebra Stripes:
 * (based on gtk3-classic patch 'treeview__alternating_row_colours.patch'):
 *
 * Adds appropriate style classes to each cell in a GtkTreeView, so that themes
 * can draw the rows of a treeview in alternating colours, also known as
 * 'zebra stripes'.
 */


#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <cairo/cairo.h>

#include <gtk3-classic.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic::Treeview-Zebra-Stripes"


static gboolean	find_row_index	(GtkTreeView *	treeview,
				 GtkTreeModel *	model,
				 GtkTreeIter *	start_iter,
				 GtkTreePath *	target_path,
				 int *		index);


INTERCEPTED_CLASS_METHOD (gtk_cell_area, render,
			  (GtkCellArea *	area,
			   GtkCellAreaContext *	context,
			   GtkWidget *		widget,
			   cairo_t *		cr,
			   const GdkRectangle *	background_area,
			   const GdkRectangle *	cell_area,
			   GtkCellRendererState	flags,
			   gboolean		paint_focus),
			  void)


void treeview_zebra_stripes_init ()
{
	GtkCellAreaClass * gtk_cell_area_class =
	 g_type_class_ref (GTK_TYPE_CELL_AREA);

	INTERCEPT_CLASS_METHOD (gtk_cell_area, GTK_CELL_AREA_CLASS,
				render)

	g_type_class_unref (gtk_cell_area_class);
}


static void
new_gtk_cell_area_render
(GtkCellArea *		area,
 GtkCellAreaContext *	context,
 GtkWidget *		widget,
 cairo_t *		cr,
 const GdkRectangle *	background_area,
 const GdkRectangle *	cell_area,
 GtkCellRendererState	flags,
 gboolean		paint_focus)
{
	GtkTreeView *		treeview;
	GtkTreeModel *		model;
	GtkStyleContext *	style;
	gint			bin_x, bin_y;
	GtkTreePath *		path;
	int			index;
	GtkTreeViewColumn *	column;
	GList *			columns;


	if (GTK_IS_TREE_VIEW (widget))
	{
		treeview = GTK_TREE_VIEW (widget);
		model = gtk_tree_view_get_model (treeview);
		style = gtk_widget_get_style_context (widget);
		columns = gtk_tree_view_get_columns (treeview);

		bin_x = cell_area->x + cell_area->width - 1;
		bin_y = cell_area->y + cell_area->height - 1;

		if (gtk_tree_view_get_headers_visible (treeview))
			gtk_style_context_add_class (style, "has-header");

		gtk_tree_view_get_path_at_pos (treeview, bin_x, bin_y,
					       &path, &column, NULL, NULL);
		if (path != NULL)
		{
			index = 0;
			find_row_index (treeview, model, NULL, path, &index);
			gtk_tree_path_free (path);
			gtk_style_context_add_class (style,
			  ((index % 2) ? "even" : "odd"));
		}

		if (column == columns->data)
			gtk_style_context_add_class (style, "first");
		if (column == g_list_last (columns)->data)
			gtk_style_context_add_class (style, "last");
		if ((flags & GTK_CELL_RENDERER_SORTED))
			gtk_style_context_add_class (style, "sorted");

		gtk_render_background (style, cr,
				       background_area->x, background_area->y,
				       background_area->width,
				       background_area->height);
	}


	CALL_ORIGINAL_CLASS_METHOD (gtk_cell_area, render,
				    (area, context, widget, cr,
				     background_area, cell_area,
				     flags, paint_focus));
}


/* Helper functions */

static gboolean
find_row_index
(GtkTreeView *	treeview,
 GtkTreeModel *	model,
 GtkTreeIter *	start_iter,
 GtkTreePath *	target_path,
 int *		index)
{
	GtkTreeIter	iter;
	GtkTreeIter	child_iter;
	GtkTreePath *	path;


	if (start_iter == NULL)
	{
		if (! gtk_tree_model_get_iter_first (model, &iter))
			return TRUE;
	}
	else
		iter = *start_iter;

	while (1)
	{
		path = gtk_tree_model_get_path (model, &iter);

		if (gtk_tree_path_compare (path, target_path) == 0)
		{
			gtk_tree_path_free (path);
			return TRUE;
		}

		if (gtk_tree_view_row_expanded (treeview, path))
		{
			(*index)++;
			gtk_tree_model_iter_children (model, &child_iter, &iter);
			if (find_row_index (treeview, model, &child_iter,
					    target_path, index))
			{
				gtk_tree_path_free (path);
				return TRUE;
			}
		}

		gtk_tree_path_free (path);
		if (! gtk_tree_model_iter_next (model, &iter))
			break;
		else
			(*index)++;
	}


	return FALSE;
}
