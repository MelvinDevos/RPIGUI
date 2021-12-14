// basic GTK program

#include <gtk/gtk.h>
#include "../PJ_RPI/PJ_RPI.h"

#define INPUT_TYPE 0
#define OUTPUT_TYPE 1

typedef struct
{
    int gpio_number;
    int state;
    int type;
} Rpi_pin;

Rpi_pin pin_list[] = {
    [0].gpio_number = 17,
    [0].state = 0,
    [0].type = OUTPUT_TYPE,

    [1].gpio_number = 27,
    [1].state = 0,
    [1].type = OUTPUT_TYPE,

    [2].gpio_number = 23,
    [2].state = 0,
    [2].type = INPUT_TYPE,

    [3].gpio_number = 24,
    [3].state = 0,
    [3].type = INPUT_TYPE};

static void check_inputs(GtkWidget *widget, gpointer data)
{
    int number_of_pins = sizeof(pin_list) / sizeof(pin_list[0]);
    char buff[80];
    for (int i = 0; i < number_of_pins; i++)
    {
        if (pin_list[i].type == INPUT_TYPE)
        {
            char input_string[20];
            INP_GPIO(pin_list[i].gpio_number);
            int state = GPIO_READ(pin_list[i].gpio_number);

            sprintf(input_string, "GPIO%d: %d ", pin_list[i].gpio_number, state);

            strcat(buff, input_string);
        }
    }

    gtk_label_set_text(GTK_LABEL(data), buff);
    printf("%s<===\n", buff);
}

static void output_state(GtkToggleButton *source, gpointer IO)
{
    int gpio_number = GPOINTER_TO_INT(IO);
    int state = gtk_toggle_button_get_active(source);
    printf("GPIO %d: %d\n", gpio_number, state);

    // GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
    INP_GPIO(gpio_number);
    OUT_GPIO(gpio_number);
    if (state)
        GPIO_SET = 1 << gpio_number;
    else
        GPIO_CLR = 1 << gpio_number;
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

    GtkWidget *btn_tgl = gtk_toggle_button_new_with_label("GPIO 27");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(btn_tgl),
                               17);
    g_signal_connect(btn_tgl, "toggled",
                     G_CALLBACK(output_state), GINT_TO_POINTER(27));

    GtkWidget *btn_tgl_2 = gtk_toggle_button_new_with_label("GPIO 22");
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(btn_tgl_2),
                               FALSE);

    g_signal_connect(btn_tgl_2, "toggled",
                     G_CALLBACK(output_state),
                     GINT_TO_POINTER(22));


    GtkWidget *lbl_input = gtk_label_new("Click button for input status!");
    GtkWidget *btn_1 = gtk_button_new_with_label("Check Inputs!");
    g_signal_connect(btn_1, "clicked",
                     G_CALLBACK(check_inputs), lbl_input);

    GtkWidget *tbl = gtk_table_new(3, 2, TRUE);

    gtk_table_attach_defaults(GTK_TABLE(tbl), btn_tgl, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(tbl), btn_tgl_2, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(tbl), btn_1, 0, 2, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(tbl), lbl_input, 0, 2, 2, 3);
    gtk_container_add(GTK_CONTAINER(window), tbl);
    gtk_widget_show_all(window);
    gtk_main();
}