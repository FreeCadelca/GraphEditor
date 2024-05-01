//
// Created by Dmitriy on 30.04.2024.
//

#include "ChooseWeightApp.h"

ChooseWeightApp::ChooseWeightApp() : ui{Gtk::Builder::create_from_file("design_choose_weight.glade")} {
    if (ui) {
        ui->get_widget<Gtk::Box>("main_box", this->main_box);
        this->weight_entry = Glib::RefPtr<Gtk::Entry>::cast_dynamic(
                ui->get_object("weight_entry")
        );
        this->button_cancel = Glib::RefPtr<Gtk::Button>::cast_dynamic(
                ui->get_object("button_cancel")
        );
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(*this, &ChooseWeightApp::on_cancel_btn_clicked));

        this->button_ok = Glib::RefPtr<Gtk::Button>::cast_dynamic(
                ui->get_object("button_ok")
        );
        this->button_ok->signal_clicked().connect(sigc::mem_fun(*this, &ChooseWeightApp::on_ok_btn_clicked));

        this->set_resizable(false);
        this->add(*this->main_box);
    }
}

void ChooseWeightApp::on_ok_btn_clicked() {

}

void ChooseWeightApp::on_cancel_btn_clicked() {

}