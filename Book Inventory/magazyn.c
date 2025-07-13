#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct ksiazka {
    unsigned id;
    char autor[255];
    char tytul[255];
    unsigned ilosc;
    float cena;
    struct ksiazka *next;
};

struct Dane {
    struct ksiazka *glowa;
    GtkWidget *entry;
    GtkWidget *output;
    GtkWidget *entry_cena;
    GtkWidget *entry_ilosc;
    GtkWidget *entry_autor;
    unsigned next_id;
};

void dodajDoListy(struct ksiazka **glowa, const char *tytul, const char *autor, unsigned ilosc, float cena, unsigned *next_id) {
    struct ksiazka *nowa = malloc(sizeof(struct ksiazka));
    if (!nowa) {
        perror("Brak pamięci\n");
        return;
    }
    nowa->id = (*next_id)++;
    strcpy(nowa->tytul, tytul);
    strcpy(nowa->autor, autor);
    nowa->ilosc = ilosc;
    nowa->cena = cena;
    nowa->next = *glowa;
    *glowa = nowa;
}

void wypelnijMagazyn(const char *nazwaPliku, struct ksiazka **glowa, unsigned *next_id) {
    FILE *fp = fopen(nazwaPliku, "r");
    if (!fp) {
        printf("Brak pliku z danymi\n");
        return;
    }

    char linia[500], tytul[255], autor[255], ilosc[20], cena[20];
    while (fgets(linia, sizeof(linia), fp)) {
        char *token = strtok(linia, ";");
        if (token) strcpy(tytul, token);
        token = strtok(NULL, ";");
        if (token) strcpy(autor, token);
        token = strtok(NULL, ";");
        if (token) strcpy(ilosc, token);
        token = strtok(NULL, ";");
        if (token) strcpy(cena, token);

        dodajDoListy(glowa, tytul, autor, atoi(ilosc), atof(cena), next_id);
    }

    fclose(fp);
}

void zapiszDoPliku(struct ksiazka *glowa) {
    FILE *fp = fopen("plik.txt", "w");
    if (!fp) {
        perror("Nie można otworzyć pliku");
        return;
    }

    struct ksiazka *obecna = glowa;
    while (obecna) {
        fprintf(fp, "%u;%s;%s;%u;%.2f\n", obecna->id, obecna->tytul, obecna->autor, obecna->ilosc, obecna->cena);
        obecna = obecna->next;
    }

    fclose(fp);
}

void wyswietlKsiazke(GtkWidget *widget, gpointer data) {
    struct Dane *dane = (struct Dane *)data;
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(dane->entry));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(dane->output));

    struct ksiazka *obecna = dane->glowa;
    while (obecna) {
        if (strcmp(obecna->tytul, input) == 0 || obecna->id == (unsigned)atoi(input)) {
            char wynik[1024];
            snprintf(wynik, sizeof(wynik), "ID: %u\nTytuł: %s\nAutor: %s\nIlość: %u\nCena: %.2f\n", 
                     obecna->id, obecna->tytul, obecna->autor, obecna->ilosc, obecna->cena);
            gtk_text_buffer_set_text(buffer, wynik, -1);
            return;
        }
        obecna = obecna->next;
    }

    gtk_text_buffer_set_text(buffer, "Nie znaleziono książki.", -1);
}

void dodajKsiazke(GtkWidget *widget, gpointer data) {
    struct Dane *dane = (struct Dane *)data;
    const gchar *tytul = gtk_entry_get_text(GTK_ENTRY(dane->entry));
    const gchar *autor = gtk_entry_get_text(GTK_ENTRY(dane->entry_autor));
    const gchar *ilosc_tekst = gtk_entry_get_text(GTK_ENTRY(dane->entry_ilosc));
    const gchar *cena_tekst = gtk_entry_get_text(GTK_ENTRY(dane->entry_cena));

    unsigned ilosc = atoi(ilosc_tekst);
    float cena = atof(cena_tekst);

    dodajDoListy(&dane->glowa, tytul, autor, ilosc, cena, &dane->next_id);
    zapiszDoPliku(dane->glowa);

    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(dane->output)), "Dodano książkę.", -1);
}

void usunKsiazke(GtkWidget *widget, gpointer data) {
    struct Dane *dane = (struct Dane *)data;
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(dane->entry));

    struct ksiazka **obecna = &dane->glowa;
    while (*obecna) {
        if (strcmp((*obecna)->tytul, input) == 0 || (*obecna)->id == (unsigned)atoi(input)) {
            struct ksiazka *do_usuniecia = *obecna;
            *obecna = (*obecna)->next;
            free(do_usuniecia);
            zapiszDoPliku(dane->glowa);
            gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(dane->output)), "Usunięto książkę.", -1);
            return;
        }
        obecna = &(*obecna)->next;
    }

    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(dane->output)), "Nie znaleziono książki do usunięcia.", -1);
}

void aktualizujKsiazke(GtkWidget *widget, gpointer data) {
    struct Dane *dane = (struct Dane *)data;
    const gchar *input = gtk_entry_get_text(GTK_ENTRY(dane->entry));
    const gchar *ilosc_tekst = gtk_entry_get_text(GTK_ENTRY(dane->entry_ilosc));
    const gchar *cena_tekst = gtk_entry_get_text(GTK_ENTRY(dane->entry_cena));

    unsigned ilosc = atoi(ilosc_tekst);
    float cena = atof(cena_tekst);

    struct ksiazka *obecna = dane->glowa;
    while (obecna) {
        if (strcmp(obecna->tytul, input) == 0 || obecna->id == (unsigned)atoi(input)) {
            obecna->ilosc = ilosc;
            obecna->cena = cena;
            zapiszDoPliku(dane->glowa);
            gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(dane->output)), "Zaktualizowano książkę.", -1);
            return;
        }
        obecna = obecna->next;
    }

    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(dane->output)), "Nie znaleziono książki do aktualizacji.", -1);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *entry;
    GtkWidget *entry_ilosc;
    GtkWidget *entry_cena;
    GtkWidget *entry_autor;
    GtkWidget *button_wyswietl;
    GtkWidget *button_dodaj;
    GtkWidget *button_usun;
    GtkWidget *button_aktualizuj;
    GtkWidget *output;

    struct Dane *dane = malloc(sizeof(struct Dane));
    dane->glowa = NULL;
    dane->next_id = 1;
    wypelnijMagazyn("plik.txt", &dane->glowa, &dane->next_id);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Magazyn książek");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 470);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Tytuł książki lub ID");
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 1, 1);
    dane->entry = entry;

    entry_autor = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_autor), "Autor");
    gtk_grid_attach(GTK_GRID(grid), entry_autor, 0, 1, 1, 1);
    dane->entry_autor = entry_autor;

    entry_ilosc = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_ilosc), "Ilość");
    gtk_grid_attach(GTK_GRID(grid), entry_ilosc, 0, 2, 1, 1);
    dane->entry_ilosc = entry_ilosc;

    entry_cena = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_cena), "Cena");
    gtk_grid_attach(GTK_GRID(grid), entry_cena, 0, 3, 1, 1);
    dane->entry_cena = entry_cena;

    button_wyswietl = gtk_button_new_with_label("Wyświetl książkę");
    g_signal_connect(button_wyswietl, "clicked", G_CALLBACK(wyswietlKsiazke), dane);
    gtk_grid_attach(GTK_GRID(grid), button_wyswietl, 0, 4, 1, 1);

    button_dodaj = gtk_button_new_with_label("Dodaj książkę");
    g_signal_connect(button_dodaj, "clicked", G_CALLBACK(dodajKsiazke), dane);
    gtk_grid_attach(GTK_GRID(grid), button_dodaj, 0, 5, 1, 1);

    button_usun = gtk_button_new_with_label("Usuń książkę");
    g_signal_connect(button_usun, "clicked", G_CALLBACK(usunKsiazke), dane);
    gtk_grid_attach(GTK_GRID(grid), button_usun, 0, 6, 1, 1);

    button_aktualizuj = gtk_button_new_with_label("Aktualizuj książkę");
    g_signal_connect(button_aktualizuj, "clicked", G_CALLBACK(aktualizujKsiazke), dane);
    gtk_grid_attach(GTK_GRID(grid), button_aktualizuj, 0, 7, 1, 1);

    output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(output), FALSE);
    gtk_widget_set_size_request(output, 400, 200);
    gtk_grid_attach(GTK_GRID(grid), output, 0, 8, 1, 1);
    dane->output = output;

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}