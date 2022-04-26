#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "benevol.h"
#include "benevol.cpp"
#include <QMessageBox>
#include <qmessagebox.h>
#include <QDebug>
#include <QtPrintSupport/QPrintDialog>
#include<QtPrintSupport/QPrinter>
#include<QPropertyAnimation>

#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrintDialog>
#include <QFileDialog>
#include <QDialog>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include<QPdfWriter>
#include<QTabWidget>
#include<QTableView>
#include <QInputDialog>
#include <QPrinter>
#include <QMediaPlayer>
#include <QThread>
#include <QUrl>
#include <QSound>
#include <QDebug>

#include <QDate>
#include <QSystemTrayIcon>
#include <iostream>
#include "parking.h"






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
        ui->lineEdit->setValidator( new QIntValidator(100, 999999, this)); //controle de saisie
        QRegExp rx("[a-zA-Z]+");
            QValidator *validator = new
                    QRegExpValidator(rx,this);



            ui->lineEdit_6->setValidator(validator);

        ui->tablebenevol->setModel(Ben.afficher());





        int ret=A.connect_arduino(); // lancer la connexion à arduino
        switch(ret){
        case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
            break;
        case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
           break;
        case(-1):qDebug() << "arduino is not available";
        }
         QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
         //le slot update_label suite à la reception du signal readyRead (reception des données).

}

MainWindow::~MainWindow()
{

    delete ui;
}
void MainWindow::on_pushButton_afficher_clicked()
{
     ui->tablebenevol->setModel(Ben.afficher());
}


void MainWindow::on_pushButton_Ajouter_clicked()
{
    int id_benevol=ui->lineEdit->text().toInt();
    QString nom_benevol=ui->lineEdit_2->text();
    QString prenom_benevol=ui->lineEdit_3->text();
    int tel_benevol=ui->lineEdit_5->text().toInt();
    QString adresse_benevol=ui->lineEdit_6->text();

     benevol Ben(id_benevol,nom_benevol,prenom_benevol,tel_benevol,adresse_benevol);
     bool test=Ben.Ajouter();

     if(test)
     {
         //Refresh(actualiser)
         ui->tablebenevol->setModel(Ben.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Click cancelto exit."),QMessageBox::Cancel);


      }
     else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                               QObject::tr("Ajout non effectué.\n"
                                           "click Cancel to exit."), QMessageBox::Cancel);

}

void MainWindow::on_pushButton_supprimer_clicked()
{
    //int id_benevol=ui->lineEdit->text().toInt();
    bool test=Ben.supprimer();
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit"), QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Suppression non effectuée\n"
                                          "Click Cancel to exit"), QMessageBox::Cancel);
}



void MainWindow::on_modifier_clicked()
{

                    int id_benevol= ui->lineEdit->text().toInt();
                    QString nom_benevol=ui->lineEdit_2->text();
                    QString prenom_benevol=ui->lineEdit_3->text();
                    int tel_benevol= ui->lineEdit_5->text().toInt();
                    QString adresse_benevol=ui->lineEdit_6->text();
                    bool test=false;

                benevol Ben(id_benevol ,nom_benevol ,prenom_benevol ,tel_benevol ,adresse_benevol);
               test=Ben.Modifier( );

                   if(test==true)
                           {
                             QMessageBox::information(nullptr,QObject::tr("ok"),
                             QObject::tr("Modifier avec success\n""click ok to exit"),QMessageBox::Ok);
                             ui->tablebenevol->setModel(Ben.afficher()); //affichage automatique apres modification


                           }
                           else
                           {

                                   QMessageBox::critical(nullptr,QObject::tr("Not ok"),
                                   QObject::tr("modification non effectue\n""click cancel to exit"),QMessageBox::Cancel);

                           }
}

void MainWindow::on_IMPRIMER_clicked()
{
    QString strStream;
                        QTextStream out(&strStream);

                        const int rowCount = ui->tablebenevol->model()->rowCount();
                        const int columnCount = ui->tablebenevol->model()->columnCount();

                        out <<  "<html>\n"
                            "<head>\n"
                            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                            <<  QString("<title>%1</title>\n").arg("strTitle")
                            <<  "</head>\n"
                            "<body bgcolor=#ffffff link=#5000A0>\n"

                           //     "<align='right'> " << datefich << "</align>"
                            "<center> <H1>Liste des Benevols </H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n";

                        // headers
                        out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
                        for (int column = 0; column < columnCount; column++)
                            if (!ui->tablebenevol->isColumnHidden(column))
                                out << QString("<th>%1</th>").arg(ui->tablebenevol->model()->headerData(column, Qt::Horizontal).toString());
                        out << "</tr></thead>\n";

                        // data table
                        for (int row = 0; row < rowCount; row++) {
                            out << "<tr> <td bkcolor=0>" << row+1 <<"</td>";
                            for (int column = 0 ; column < columnCount; column++) {
                                if (!ui->tablebenevol->isColumnHidden(column)) {
                                    QString data = ui->tablebenevol->model()->data(ui->tablebenevol->model()->index(row, column)).toString().simplified();
                                    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                                }
                            }
                            out << "</tr>\n";
                        }
                        out <<  "</table> </center>\n"
                            "</body>\n"
                            "</html>\n";

                  QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Sauvegarder en PDF", QString(), "*.pdf");
                    if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

                   QPrinter printer (QPrinter::PrinterResolution);
                    printer.setOutputFormat(QPrinter::PdfFormat);
                   printer.setPaperSize(QPrinter::A4);
                  printer.setOutputFileName(fileName);

                   QTextDocument doc;
                    doc.setHtml(strStream);
                    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
                    doc.print(&printer);
}



void MainWindow::on_MEMBRE_clicked()
{
       participer *par =new participer();
    par->membermoi();
       QString con= QString::number(par->get_count());
         QString i_d= QString::number(par->get_benevol());
     ui->id->setText(i_d);
      ui->nbrpart->setText(con);

}



void MainWindow::on_tablebenevol_activated() //    void on_tablebenevol_activated();

{

    //int id_benevol=ui->lineEdit->text().toInt();
    QString nom_benevol=ui->lineEdit_2->text();
    QString prenom_benevol=ui->lineEdit_3->text();
    //int tel_benevol=ui->lineEdit_5->text().toInt();
    QString adresse_benevol=ui->lineEdit_6->text();


            benevol Ben;



           ui->lineEdit->setText(QString(QString::number(Ben.getid_benevol())));
           ui->lineEdit_2->setText(QString(Ben.getnom_benevol()));
           ui->lineEdit_3->setText(QString(Ben.getprenom_benevol()));
           ui->lineEdit_5->setText(QString(Ben.gettel_benevol()));
           ui->lineEdit_6->setText(QString(Ben.gettel_benevol()));

}

void MainWindow::on_UPLOAD_clicked()
{
Video=new video(this);
    Video->show();
}

void MainWindow::on_TRIER_clicked()
{
    ui->tablebenevol->setModel(Ben.trier_id_benevol());

}

void MainWindow::on_lineEdit_4_textEdited()
{
    /*QString nom=ui->lineEdit_4->text();
           // qDebug()<<nom;
           ui->tablebenevol->setModel(Ben.rechercher(nom));*/


}

void MainWindow::on_rechercher_clicked()
{
    QString nom=ui->lineEdit_4->text();
           // qDebug()<<nom;
           ui->tablebenevol->setModel(Ben.rechercher(ui->lineEdit_4->text()));

}

void MainWindow::on_stat_clicked()
{

    Stat *w = new Stat();
    w->make();
    w->show();
}




void MainWindow::update_label()
{
    /*parking P ;
     data=A.read_from_arduino();

      QString Sstring = QString(data);
      Sstring.remove("\r\n");
      ui->label_3->setText(data);

      qDebug() << Sstring;
       bool test=P.verifier(Sstring);
      if (test){
         A.write_to_arduino("0");
      }
      else
          A.write_to_arduino("1");*/
}

void MainWindow::on_pushButton_clicked()
{
   parking P ;
    data=A.read_from_arduino();

     QString Sstring = QString(data);
     Sstring.remove("\r\n");
     ui->label_3->setText(data);

     qDebug() << Sstring;
      bool test=P.verifier(Sstring);
     if (test){
        A.write_to_arduino("0");
     }
     else
         A.write_to_arduino("1");
}
