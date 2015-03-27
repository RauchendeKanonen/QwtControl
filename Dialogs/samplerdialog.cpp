#include "samplerdialog.h"
#include "ui_samplerdialog.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "csv.h"
#include "QFileDialog"
#include "QMessageBox"
#include "csv.h"
#include "csvdialog.h"


SamplerDialog::SamplerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SamplerDialog)
{
    ui->setupUi(this);

    setWindowTitle("Response");
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode( QwtLegend::ClickableItem );
    ui->qwtPlot->insertLegend( legend, QwtPlot::RightLegend );

    connect( ui->qwtPlot, SIGNAL(legendClicked(QwtPlotItem*)),
             this, SLOT( legendClicked( QwtPlotItem *) ) );


    Curve = new QwtPlotCurve("Sampled Curve");


    QwtSymbol Symbol( QwtSymbol::Rect,
                      QColor("red"), QColor("red"), QSize( 3, 3 ) );
    Symbol.setPen(QPen(QColor("black")));
    Curve->setSymbol(Symbol);
    Curve->attach(ui->qwtPlot);
    YData = (double*)malloc(1000*sizeof(double));
    XData = (double*)malloc(1000*sizeof(double));
}

SamplerDialog::~SamplerDialog()
{
    delete ui;
}

void SamplerDialog::insertCurve(QwtControlPlotItem *Item)
{
    if(QwtPlotItem::Rtti_PlotUserItem+Rtti_PlotResponse == Item->rtti())
    {
        connect((QwtResponseCurve*)Item, SIGNAL(dataReadySig(QPolygonF)),this, SLOT(dataSlot(QPolygonF)));
    }
}


void SamplerDialog::reSample(void)
{

    double Ts = ui->doubleSpinBoxTs->value();
    int NDots = ui->spinBoxN->value();
    SampledPolygon.clear();


    for(int i = 0 ; i < NDots && i < SrcPolygon.count() ; i ++ )
    {
        for(int c = 0 ; c < SrcPolygon.count() ; c ++ )
        {
            int x = round(SrcPolygon.at(c).x()/Ts);
            if(x == i)
            {
                SampledPolygon.append(SrcPolygon.at(c));
                break;
            }
        }
    }
    Curve->setData(SampledPolygon);
    Curve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();


}

void SamplerDialog::dataSlot(QPolygonF PolygonA)
{
    SrcPolygon = PolygonA;
    reSample();
}

void SamplerDialog::on_pushButtonSample_clicked()
{
    reSample();
}



void SamplerDialog::on_pushButtonXmGrace_clicked()
{
    csv CSVFile("export/default.dat");
    CSVFile.setData(&SampledPolygon);
    CSVFile.writeCSV();
    system("xmgrace export/default.dat");
}

void SamplerDialog::on_pushButtonDeq_clicked()
{
    QString Rationale;
    double Sum = 0;
    double SrcSum = 0;
    //sum(y[k]*z^-(k-1),k,1,101);
    for(int i = 0 ; i < SrcPolygon.count() ; i ++ )
        SrcSum+=SrcPolygon.at(i).y();

    for(int i = 0 ; i < SampledPolygon.count() ; i ++ )
    {
        QString Part;
        Part.sprintf("%f*z^(-%d)", SampledPolygon.at(i).y(), i);
        Sum += fabs(SampledPolygon.at(i).y());
        Rationale+="+"+Part;
    }

    QString Normalizer;
    Normalizer.sprintf("/%f", Sum);

    ex Deq = inversZTransform("("+Rationale+")"+Normalizer);
    emit newExpression(Rationale+Normalizer);


    ostringstream DiffStr;
    DiffStr << Deq;


    QString DifferenceEquation(DiffStr.str().c_str());

    emit newExpression(DifferenceEquation);
}

ex SamplerDialog::inversZTransform(QString RationalFunction)
{
    symbol x("x");
    symbol y("y");
    symbol *y0;
    symbol s;

    GiNaC::parser reader;
    GiNaC::ex Rational;
    try
    {
        Rational = reader(RationalFunction.toStdString());
    }
    catch (exception &p)
    {
        QMessageBox Box;
        Box.setText(p.what());
        Box.setModal(true);
        Box.exec();
        return GiNaC::ex();
    }

    GiNaC::symtab table = reader.get_syms();
    s = table.find("z") != table.end() ?
                GiNaC::ex_to<GiNaC::symbol>(table["z"]) : GiNaC::symbol("z");





    ex Numerator = Rational.numer();
    ex DeNumerator = Rational.denom();

    ex Polynome = DeNumerator;

    Polynome /= ex(pow(s, Polynome.expand().degree(s)));
    ex Poly = Polynome.expand();
    ex YTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;
        if(i!=0)
            SymbolStr.sprintf("%s[i%d]",y.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s[i]",y.get_name().c_str());

        symbol *ydq = new symbol(SymbolStr.toStdString());
        if(i==0)
            y0 = ydq;
        YTerm += *ydq*Poly.coeff(s,i);
    }


    Polynome = Numerator;
    Polynome /= ex(pow(s, Polynome.expand().degree(s)));
    Poly = Polynome.expand();
    ex XTerm;

    for (int i=Poly.ldegree(s); i<=Poly.degree(s); ++i)
    {
        QString SymbolStr;


        if(i!=0)
            SymbolStr.sprintf("%s[i%d]",x.get_name().c_str(),i);
        else
            SymbolStr.sprintf("%s[i]",x.get_name().c_str());

        symbol *xdq = new symbol(SymbolStr.toStdString());
        XTerm += *xdq*Poly.coeff(s,i);
    }

    ex DifferenceEquation = YTerm==XTerm;

    DifferenceEquation = lsolve(DifferenceEquation, *y0);
    return DifferenceEquation;
}

void SamplerDialog::on_pushButtonCSV_clicked()
{
    QFileDialog dlg(this, QString("Laden"),QString("data/"));
    dlg.setModal(true);

    if(dlg.exec())
    {
        if(dlg.selectedFiles().count())
        {
            csv CSV(dlg.selectedFiles().first());
            CSVDialog Dlg(this, &CSV);
            Dlg.setModal(true);
            if(!Dlg.exec())
                return;
            CSV.readCSV(-1);
            QPolygonF Polygon = CSV.getData();
            dataSlot(Polygon);
        }
    }
}
