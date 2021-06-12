/* GNU General Public License v3.0 */
/******************************************************************************
 *
 * Name: main.c - Main source code for GtkRunes project 
 *
 * Copyright (C) 2021, binary_cat
 *
 *****************************************************************************/

#include <gtk/gtk.h>
#include <ctype.h>
#include "GtkRunes.h"

typedef struct {

    GtkWidget * img_main;

} app_widgets;

gchar * image_name;

app_widgets * widgets;

int i_CurrentIndex; 

GdkPixbuf *create_pixbuf(const gchar * filename) {

    GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);

	if (!pixbuf) {

	   fprintf(stderr, "%s\n", error->message);
	   g_error_free(error);

	   }
	
	return pixbuf;
}

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
	GdkPixbuf *icon;

    widgets = g_slice_new(app_widgets);
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file(GTK_GUI_CONSTRUCTION);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    widgets->img_main = GTK_WIDGET(gtk_builder_get_object(builder,"img_holder"));

	icon = create_pixbuf("data/r_app_icon.png");
	gtk_window_set_icon(GTK_WINDOW(window), icon);

    image_name = "data/1.png";
    gtk_image_set_from_file(GTK_IMAGE(widgets->img_main),image_name);

    i_CurrentIndex = 0;
    
    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show_all(window);                
    gtk_main();
    
    g_slice_free(app_widgets, widgets);

    return 0;
}

void on_window_main_destroy()
{

    gtk_main_quit();

}

void on_checking_button_clicked(GtkButton *button, GtkEntry *entry)
{

    char * enteredText = gtk_entry_get_text(entry);

    for (int i = 0;i<=strlen(enteredText);i++)
            enteredText[i] = tolower(enteredText[i]);

    if((strcmp(enteredText,p_RunesArray[i_CurrentIndex])==0)||
        (strcmp(enteredText,p_RunesArray_RUS[i_CurrentIndex])==0)||
        (strcmp(enteredText,p_RunesArray_RUS_ver2[i_CurrentIndex])==0))

        {

        gtk_button_set_label(button, "OK");

        unsigned int seed;
        FILE* urandom = fopen("/dev/urandom", "r");
        fread(&seed, sizeof(int), 1, urandom);
        fclose(urandom);
        srand(seed);

        i_CurrentIndex = rand()%25;
        char str_number[2];
        sprintf(str_number,"%d",i_CurrentIndex);
        char * str_pointer_image_name = malloc(strlen(str_number)+10);

        strcpy(str_pointer_image_name,"data/");
        strcat(str_pointer_image_name,str_number);
        strcat(str_pointer_image_name,".png");

        image_name = str_pointer_image_name;

        gtk_image_set_from_file(GTK_IMAGE(widgets->img_main),image_name);
        free(str_pointer_image_name); 
        i_CurrentIndex--; 
        gtk_entry_set_text(entry,"");

         } 

    else {

        gtk_entry_set_text (entry, "");
        
        gtk_button_set_label(button,"Wrong answer, try again");

        }
}

void on_entry_box_activate(GtkEntry * entry, GtkButton *button)
{

    on_checking_button_clicked(button,entry);

}
