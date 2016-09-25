/*
 * Copyright (C) 2016  Elvis Teixeira
 *
 * This source code is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include <slope/slope.h>
#include <math.h>


#define NMOL_SQRT    40
#define BOX_EDGE    10.0
#define MAX_INITIAL_SPEED   10.0
#define TIME_STEP    1.0e-3
#define TIMEOUT   340
#define STEPS_PER_TIMEOUT   10
#define G_MAX_SAMPLES    60

#define NMOL    (NMOL_SQRT*NMOL_SQRT)
#define BOX_EDGE_DIV    (BOX_EDGE/NMOL_SQRT)
#define HALF_TIME_STEP_SQR    (TIME_STEP*TIME_STEP*0.5)
#define MAX_BOX_COORD    (BOX_EDGE_DIV*(NMOL_SQRT+1))
#define BOX_DIAGONAL    (MAX_BOX_COORD*M_SQRT2)
#define G_SAMPLE_SPACING    (MAX_BOX_COORD/G_MAX_SAMPLES)


GtkWidget *chart;
GtkWidget *view;
SlopeScale *mol_scale;
SlopeItem *mol_plot;
SlopeScale *g_scale;
SlopeItem *g_plot;

double *x, *y;
double *vx, *vy;
double *ax, *ay;
double *m;
double system_time;
double *g_x, *g_y;



void incr_g_bin (double d)
{
    unsigned long int k;

    k = (d / BOX_DIAGONAL) * G_MAX_SAMPLES;

    g_y[k] += 1.0;
}



void calc_forces_runge_kutta ()
{
    unsigned long int i, j;
    double dx, dy, d, d2;

    for (i=0UL; i<G_MAX_SAMPLES; ++i) {
        g_y[i] = 0.0;
    }

    for (i=0UL; i<NMOL; ++i) {
        for (j=i+1UL; j<NMOL; ++j)
        {
            dx = x[j] - x[i];
            dy = y[j] - y[i];

            d2 = dx*dx + dy*dy;
            d = sqrt(d2);

            incr_g_bin(d);
        }
    }
}



void create_initial_conditions ()
{
    unsigned long int i, j, k;
    double v, t;

    for (i=0UL; i<G_MAX_SAMPLES; ++i)
    {
        g_x[i] = i;
        g_y[i] = 0.0;
    }

    k=0UL;
    for (i=0UL; i<NMOL_SQRT; ++i) {
        for (j=0UL; j<NMOL_SQRT; ++j)
        {
            x[k] = (1 + i) * BOX_EDGE_DIV;
            y[k] = (1 + j) * BOX_EDGE_DIV;

            v = g_random_double_range(0.0, MAX_INITIAL_SPEED);
            t = g_random_double_range(0.0, 2.0*G_PI);

            vx[k] = v * cos(t);
            vy[k] = v * sin(t);

            ax[k] = 0.0;
            ay[k] = 0.0;

            k += 1;
        }
    }

    calc_forces_runge_kutta();
}



void create_system ()
{
    x = g_malloc(NMOL * sizeof(double));
    y = g_malloc(NMOL * sizeof(double));

    vx = g_malloc(NMOL * sizeof(double));
    vy = g_malloc(NMOL * sizeof(double));

    ax = g_malloc(NMOL * sizeof(double));
    ay = g_malloc(NMOL * sizeof(double));

    m = g_malloc(NMOL * sizeof(double));

    g_x = g_malloc(G_MAX_SAMPLES * sizeof(double));
    g_y = g_malloc(G_MAX_SAMPLES * sizeof(double));
}



void destroy_system ()
{
    g_free(x);
    g_free(y);

    g_free(vx);
    g_free(vy);

    g_free(ax);
    g_free(ay);

    g_free(m);

    g_free(g_x);
    g_free(g_y);
}



void closed_boundary_condition (unsigned long int k)
{
    /* makes the particles bounce back on coliding with
       the box walls */

    if (x[k] < 0.0) vx[k] = -vx[k];
    if (y[k] < 0.0) vy[k] = -vy[k];

    if (x[k] > MAX_BOX_COORD) vx[k] = -vx[k];
    if (y[k] > MAX_BOX_COORD) vy[k] = -vy[k];
}



void euler_integration_step ()
{
    unsigned long int k;
    /* GTimer *timer = g_timer_new (); */

    calc_forces_runge_kutta();

    for (k=0UL; k<NMOL; ++k)
    {
        vx[k] += ax[k] * TIME_STEP;
        vy[k] += ay[k] * TIME_STEP;

        x[k] += vx[k] * TIME_STEP + ax[k] * HALF_TIME_STEP_SQR;
        y[k] += vy[k] * TIME_STEP + ay[k] * HALF_TIME_STEP_SQR;

        closed_boundary_condition(k);
    }

    system_time += TIME_STEP;

    /* gulong microsecs;
    double secs = g_timer_elapsed(timer, &microsecs);
    g_print("time of a step: %lf\n", secs);
    g_timer_destroy(timer); */
}



gboolean animation_timeout (gpointer data)
{
    int k;
    SLOPE_UNUSED(data);

    for (k=0; k<STEPS_PER_TIMEOUT; ++k) {
        euler_integration_step();
    }

    slope_xyseries_update(SLOPE_XYSERIES(g_plot));
    slope_view_redraw(SLOPE_VIEW(view));

    return TRUE;
}



void simulation_entry_point ()
{
    g_timeout_add(TIMEOUT, animation_timeout, NULL);
}



void create_window ()
{
    chart = slope_chart_new_detailed("Molecules", 500, 620);
    view = slope_chart_get_view(SLOPE_CHART(chart));
    g_signal_connect(G_OBJECT(chart), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    slope_chart_set_show_animation_button(SLOPE_CHART(chart), TRUE);
    g_signal_connect(G_OBJECT(
        slope_chart_get_animation_button(SLOPE_CHART(chart))),
                     "clicked", G_CALLBACK(simulation_entry_point), NULL);

    mol_scale = slope_xyscale_new_axis("Molecules", NULL);
    slope_xyscale_set_visible_axis(SLOPE_XYSCALE(mol_scale), SLOPE_XYSCALE_FRAME_LINE_TITLE);
    slope_scale_set_layout_rect(mol_scale, 0, 0, 1, 3);
    slope_chart_add_scale(SLOPE_CHART(chart), mol_scale);

    mol_plot = slope_xyseries_new_filled("molecules", x, y, NMOL, "bo");
    slope_scale_add_item(mol_scale, mol_plot);
    slope_xyscale_set_x_range(SLOPE_XYSCALE(mol_scale), 0.0, MAX_BOX_COORD);
    slope_xyscale_set_y_range(SLOPE_XYSCALE(mol_scale), 0.0, MAX_BOX_COORD);

    g_scale = slope_xyscale_new_axis("G(r)", NULL);
    slope_xyscale_set_visible_axis(SLOPE_XYSCALE(g_scale), SLOPE_XYSCALE_FRAME_LINE_TITLE);
    slope_scale_set_layout_rect(g_scale, 0, 3, 1, 1);
    slope_chart_add_scale(SLOPE_CHART(chart), g_scale);

    g_plot = slope_xyseries_new_filled("G Function", g_x, g_y, G_MAX_SAMPLES, "r-");
    slope_scale_add_item(g_scale, g_plot);
}



void run_application ()
{
    gtk_widget_show_all(chart);
    gtk_main();
}



int main (int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    create_system();
    create_initial_conditions();
    create_window();

    run_application();

    destroy_system();
    return 0;
}
