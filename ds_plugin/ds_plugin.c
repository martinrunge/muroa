/*
 *  Copyright 2000 H�ard Kv�en <havardk@sol.no>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <glib.h>
#include <gtk/gtk.h>
#include <xmms/plugin.h>
#include <xmms/util.h>
#include <xmms/configfile.h>

#include <stdlib.h> 
#include <stdio.h>

#include "ds_cpp_plugin.h"

static GTimer *timer;
static gulong offset_time, written;
static gint bps;
static gboolean real_time = TRUE;
static gboolean paused, started;
static GtkWidget *configurewin;
static GtkWidget *client_hbox[8], *client_label[8], *client_entry[8];
static char **clients;

static struct {
	AFormat format;
	gint frequency;
	gint channels;
} input_format;

#define ELAPSED_TIME (offset_time + g_timer_elapsed(timer, NULL) * 1000)

static void ds_init(void)
{
  int i;
	ConfigFile *cfg;
  clients = (char**)malloc(9*sizeof(char*));
  for(i=0; i < 9; i++) clients[i] = NULL;

	cfg = xmms_cfg_open_default_file();
	xmms_cfg_read_boolean(cfg, "ds", "real_time", &real_time);
  xmms_cfg_read_string(cfg, "ds", "ds_client0", clients);
  xmms_cfg_read_string(cfg, "ds", "ds_client1", clients+1);
  xmms_cfg_read_string(cfg, "ds", "ds_client2", clients+2);
  xmms_cfg_read_string(cfg, "ds", "ds_client3", clients+3);
  xmms_cfg_read_string(cfg, "ds", "ds_client4", clients+4);
  xmms_cfg_read_string(cfg, "ds", "ds_client5", clients+5);
  xmms_cfg_read_string(cfg, "ds", "ds_client6", clients+6);
  xmms_cfg_read_string(cfg, "ds", "ds_client7", clients+7);
	xmms_cfg_free(cfg);
  for(i=0; i < 8; i++)
    printf("clients[%d] = %x\n", i, clients[i]);
  ds_cpp_init(clients);
}

static void ds_about(void)
{
	static GtkWidget *about = NULL;

	if (about)
		return;
	about = xmms_show_message("About DS-Plugin ",
				  "Output plugin for the DS-Player streaming engine" 
				  " by Martin Runge <martin.runge@web.de>",
				  "Ok", FALSE, NULL, NULL);
	gtk_signal_connect(GTK_OBJECT(about), "destroy",
			   GTK_SIGNAL_FUNC(gtk_widget_destroyed),
			   &about);
}

static void ds_configure_ok_cb(GtkButton *w, gpointer data)
{
	ConfigFile *cfg;
  int i;
  char buffer[64];

	real_time = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(data));
	cfg = xmms_cfg_open_default_file();
	xmms_cfg_write_boolean(cfg, "ds", "real_time", real_time);

  for(i=0; i < 8; i++) {
/*    if(gtk_entry_get_text(client_entry[i]) != NULL) {*/

      if (clients[i])
        g_free(clients[i]);
      clients[i] = g_strdup(gtk_entry_get_text(GTK_ENTRY(client_entry[i])));
      sprintf(buffer, "ds_client%d", i);
      printf("client %d entry: %s=%s \n", i, buffer, clients[i]);
      xmms_cfg_write_string(cfg, "ds", buffer, clients[i] );
    /*}*/
  }

	xmms_cfg_write_default_file(cfg);
	xmms_cfg_free(cfg);
	gtk_widget_destroy(configurewin);
  ds_cpp_config_ok(clients);
}


static void ds_configure(void)
{
	GtkWidget *rt_btn, *ok_button, *cancel_button, *vbox, *bbox;
  int i;
  char client_label_str[64];

	if (configurewin)
		return;
	
	configurewin = gtk_window_new(GTK_WINDOW_DIALOG);
	gtk_window_set_title(GTK_WINDOW(configurewin), "DS output preferences");
	gtk_window_set_policy(GTK_WINDOW(configurewin), FALSE, FALSE, FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(configurewin), 10);
	gtk_signal_connect(GTK_OBJECT(configurewin), "destroy",
			   GTK_SIGNAL_FUNC(gtk_widget_destroyed), &configurewin);

	vbox = gtk_vbox_new(FALSE, 10);
	gtk_container_add(GTK_CONTAINER(configurewin), vbox);

	rt_btn = gtk_check_button_new_with_label("Run in real time");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rt_btn), real_time);
	gtk_box_pack_start(GTK_BOX(vbox), rt_btn, FALSE, FALSE, 0);

  for(i=0; i < 8; i++) {
    client_hbox[i] = gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), client_hbox[i], FALSE, FALSE, 0);


    sprintf(client_label_str, "Client %d:", i);
    client_label[i] = gtk_label_new(client_label_str);
    gtk_box_pack_start(GTK_BOX(client_hbox[i]), client_label[i], FALSE, FALSE, 0);
    gtk_widget_show(client_label[i]);

    client_entry[i] = gtk_entry_new();
    if (clients[0] != NULL)
        gtk_entry_set_text(GTK_ENTRY(client_entry[i]), clients[i]);
    gtk_widget_set_usize(client_entry[i], 100, -1);
    gtk_box_pack_start(GTK_BOX(client_hbox[i]), client_entry[i], TRUE, TRUE, 0);
    gtk_widget_show(client_entry[i]);

  }

	bbox = gtk_hbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
	gtk_button_box_set_spacing(GTK_BUTTON_BOX(bbox), 5);
	gtk_box_pack_start(GTK_BOX(vbox), bbox, FALSE, FALSE, 0);
	ok_button = gtk_button_new_with_label("Ok");
	cancel_button = gtk_button_new_with_label("Cancel");
	GTK_WIDGET_SET_FLAGS(ok_button, GTK_CAN_DEFAULT);
	GTK_WIDGET_SET_FLAGS(cancel_button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default(ok_button);
	gtk_signal_connect_object(GTK_OBJECT(cancel_button), "clicked",
				  GTK_SIGNAL_FUNC(gtk_widget_destroy),
				  GTK_OBJECT(configurewin));
	gtk_signal_connect(GTK_OBJECT(ok_button), "clicked",
			   GTK_SIGNAL_FUNC(ds_configure_ok_cb), rt_btn);
	gtk_box_pack_start_defaults(GTK_BOX(bbox), ok_button);
	gtk_box_pack_start_defaults(GTK_BOX(bbox), cancel_button);

	gtk_widget_show_all(configurewin);
}

static int ds_open(AFormat fmt, int rate, int nch)
{
  int retval; 
  /*fprintf(stderr,"Format %d, rate %d, num channels %d", fmt, rate, nch); */
  retval = ds_cpp_open(2 * rate * nch);
  if(retval == 0)
    return 1;
  else
    return 0;
}

static void ds_write(void *ptr, int length)
{
	EffectPlugin *ep;

	if ((ep = get_current_effect_plugin()) != NULL &&
	    effects_enabled() && ep->mod_samples)
		ep->mod_samples(&ptr, length, input_format.format,
				input_format.frequency, input_format.channels);
	

  ds_cpp_write(ptr, length);
}

static void ds_close(void)
{
  ds_cpp_close();
}

static void ds_flush(int time)
{
  ds_cpp_flush();
}

static void ds_pause(short p)
{
  ds_cpp_pause();
}

static int ds_buffer_free(void)
{
  return 10240;
}

static int ds_playing(void)
{
	if (!timer)
		return FALSE;
	
	if ((gdouble)(written * 1000) / bps > ELAPSED_TIME)
		return TRUE;
	return FALSE;
}

static int ds_get_written_time(void)
{
	if (!bps)
		return 0;
	return ((gint64)written * 1000) / bps;
}

static int ds_get_output_time(void)
{
	if (!timer)
		return ds_get_written_time();
	
	return ELAPSED_TIME;
}

OutputPlugin ds_op =
{
	NULL,
	NULL,
	"DS-Player Engine ",
	ds_init,
	ds_about,
	ds_configure,
	NULL,			/* Get volume */
	NULL,			/* Set volume */
	ds_open,
	ds_write,
	ds_close,
	ds_flush,
	ds_pause,
	ds_buffer_free,
	ds_playing,
	ds_get_output_time,
	ds_get_written_time,
};

OutputPlugin *get_oplugin_info(void)
{
	return &ds_op;
}
