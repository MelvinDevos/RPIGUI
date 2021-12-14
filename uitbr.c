// basic GTK program

#include <gtk/gtk.h>
#include "../PJ_RPI/PJ_RPI.h"
#include "string.h"

#define INPUT_TYPE 0
#define OUTPUT_TYPE 1

int output_gpio_number = 0;
int input_gpio_number = 0;
int can_read_input = 0;
int can_read_output = 0;

static void set_input(GtkWidget *widget, gpointer data)
{
    can_read_input = 1;
    input_gpio_number = atoi(gtk_entry_get_text(GTK_ENTRY(data)));
}

static void set_output(GtkWidget *widget, gpointer data)
{
    can_read_output = 1;
    output_gpio_number = atoi(gtk_entry_get_text(GTK_ENTRY(data)));
}
static void check_input(GtkWidget *widget, gpointer data)
{
    char buff[35];

    if (can_read_input)
    {

        INP_GPIO(input_gpio_number);

        int state = 0;
        if (GPIO_READ(input_gpio_number))
            state = 1;

        sprintf(buff, "GPIO%d: %d ", input_gpio_number, state);

        gtk_label_set_text(GTK_LABEL(data), buff);
        printf("%s<===\n", buff);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(data), "Set input GPIO first");
        printf("%s<===\n", buff);
    }
}

static void toggle_output(GtkToggleButton *source, gpointer IO)
{
    if (can_read_input)
    {
        int state = gtk_toggle_button_get_active(source);
        printf("GPIO %d: %d\n", output_gpio_number, state);

        // GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
        INP_GPIO(output_gpio_number);
        OUT_GPIO(output_gpio_number);
        if (state)
            GPIO_SET = 1 << output_gpio_number;
        else
            GPIO_CLR = 1 << output_gpio_number;
    }
}

int main(int argc, char *argv[])
{

    if (map_peripheral(&gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        return -1;
    }

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Label for displaying the GPIO state
    GtkWidget *lbl_input = gtk_label_new("Click button for input status!");

    GtkObject *adj = gtk_adjustment_new(0, 0, 27, 1, 0, 0);
    GtkWidget *gpio_input_number = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0, 0);
    GtkWidget *gpio_output_number = gtk_spin_button_new(GTK_ADJUSTMENT(adj), 0, 0);

    GtkWidget *btn_tgl_output = gtk_toggle_button_new_with_label("GPIO TOGGLE");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(btn_tgl_output),
                               17);

    g_signal_connect(btn_tgl_output, "toggled",
                     G_CALLBACK(toggle_output),
                     GINT_TO_POINTER(output_gpio_number));

    GtkWidget *btn_input = gtk_button_new_with_label("Check Input!");
    g_signal_connect(btn_input, "clicked",
                     G_CALLBACK(check_input), lbl_input);

    GtkWidget *btn_set_input = gtk_button_new_with_label("Set Input!");
    g_signal_connect(btn_set_input, "clicked",
                     G_CALLBACK(set_input), gpio_input_number);

    GtkWidget *btn_set_output = gtk_button_new_with_label("Set Output!");
    g_signal_connect(btn_set_output, "clicked",
                     G_CALLBACK(set_output), gpio_output_number);

    GtkWidget *tbl = gtk_table_new(7, 2, TRUE);

    gtk_table_attach_defaults(GTK_TABLE(tbl), gpio_input_number, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(tbl), btn_set_input, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(tbl), btn_input, 0, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(tbl), lbl_input, 0, 2, 2, 3);

    gtk_table_attach_defaults(GTK_TABLE(tbl), gpio_output_number, 0, 1, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(tbl), btn_set_output, 1, 2, 4, 5);
    gtk_table_attach_defaults(GTK_TABLE(tbl), btn_tgl_output, 0, 2, 6, 7);
    gtk_container_add(GTK_CONTAINER(window), tbl);
    gtk_widget_show_all(window);
    gtk_main();
}