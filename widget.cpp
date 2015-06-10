#include "widget.h"
#include "ui_widget.h"
#include <QtWidgets>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    parser();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::parser()
{
    // open the hardcoded mission.sqm ;)
    QFile file("d://test//mission-for-parser.sqm");

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"),
        file.errorString());
        return;
    }

    QStringList list;
    QString line;
    QTextStream in(&file);

    while (!in.atEnd())
    {
        line = in.readLine();

        // then loop the markers
        if (line.contains("position[]="))
        {
            // parse position coordinates
            line.replace("position[]={", "");
            line.replace("};", "");
            list = line.split(",");

            QString loctype,classtype;
            QString pos = "position[]={";
            // prepare the 2D coordinates X,Y format string
            pos.append(list[0]);
            pos.append(",");
            pos.append(list[2]);
            pos.append("};");

            // read name which will be our NAME type umm
            line = in.readLine();
            if (line.contains("name="))
            {
                    // and remove the name and ending "; etc.
                    line.replace("name=\"", "");
                    line.replace("\"", "");
                    line.replace(";", "");

                    // check if we have matching marker, then choose proper class name type
                    if (line.contains("StrongpointArea"))
                    {
                        loctype = "StrongpointArea";
                        classtype = "AStrong_";
                    }

                    // this first as its shorter
                    // also after this:  city38_FlatAreaCity
                    // add some method to insert the city name and hmm DUNNO! :)
                    if (line.contains("FlatAreaCity"))
                    {
                        loctype = "FlatAreaCity";
                        classtype = "AFlatC_";
                    }

                    // and if its citysmall, then we do this
                    if (line.contains("FlatAreaCitySmall"))
                    {
                        loctype = "FlatAreaCitySmall";
                        classtype = "AFlatCS_";
                    }

                    // hilltop
                    if (line.contains("pmc_hill"))
                    {
                        loctype = "Hill";
                        classtype = "hill_";
                    }

                    // airport
                    if (line.contains("pmc_airport"))
                    {
                        loctype = "Airport";
                        classtype = "AAirport_";
                    }

                    // marine
                    if (line.contains("pmc_marine"))
                    {
                        loctype = "NameMarine";
                        classtype = "Marine_";
                    }

                    /*
                    this is stupid as it will miss "name="FlatArea";" completely :(
                    but if we don't do it with underscore, then it picks up FlatAreaCity and FlatAreaCitySmall too :(
                    if you strip the name=" away after these checks, then you could compare to "name="FlatArea"" string.
                    */
                    if (line.contains("FlatArea_"))
                    {
                        loctype = "FlatArea";
                        classtype = "AFlat_";
                    }

                    // class name
                    QString buf = "class ";
                    buf.append(classtype);
                    line.replace("\t\t\t", "");
                    buf.append(line);
                    buf.append("\n");

                    // { open
                    buf.append("{\nname=\"\";\n");

                    // position
                    pos.replace("\t\t\t", "");
                    buf.append(pos);
                    buf.append("\n");

                    QString fucker = "type=\"" + loctype + "\";\n";
                    //buf.append("type=\"\";\n");
                    buf.append(fucker);
                    buf.append("radiusA = 100;\n");
                    buf.append("radiusB = 100;\n");
                    buf.append("angle = 0;\n");

                    // } close
                    buf.append("};\n\n");

                    // print the output
                    ui->textEdit->append(buf);
            }
        }
    }
    file.close();
}
