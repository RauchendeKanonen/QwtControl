#include "bodedialog.h"
#include "ui_bodedialog.h"
#include "mainwindow.h"



BodeDialog::BodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BodeDialog)
{
    ui->setupUi(this);
    pParent = parent;

    AmplitudeMarker = new QwtPlotMarker();
    AmplitudeMarker->setLineStyle( QwtPlotMarker::HLine );
    AmplitudeMarker->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    AmplitudeMarker->setLinePen( QPen( QColor( 200, 150, 0 ), 0, Qt::DashDotLine ) );
    AmplitudeMarker->setSymbol( QwtSymbol( QwtSymbol::Diamond,
        QColor( Qt::yellow ), QColor( Qt::green ), QSize( 7, 7 ) ) );


    PhaseMarker = new QwtPlotMarker();
    PhaseMarker->setLineStyle( QwtPlotMarker::HLine );
    PhaseMarker->setLabelAlignment( Qt::AlignRight | Qt::AlignBottom );
    PhaseMarker->setLinePen( QPen( QColor( 200, 150, 0 ), 0, Qt::DashDotLine ) );
    PhaseMarker->setSymbol( QwtSymbol( QwtSymbol::Diamond,
        QColor( Qt::yellow ), QColor( Qt::green ), QSize( 7, 7 ) ) );



    ui->qwtAmplitudePlot->setAxisScaleEngine( QwtPlot::yLeft, new QwtLog10ScaleEngine );
    ui->qwtAmplitudePlot->setAxisScaleEngine( QwtPlot::xBottom, new QwtLog10ScaleEngine );
    ui->qwtPhasePlot->setAxisScaleEngine( QwtPlot::xBottom, new QwtLog10ScaleEngine );
    AmplitudeCurve = new QwtPlotCurve( "Amplidue" );
    PhaseCurve = new QwtPlotCurve( "Phase" );
    AmplitudeCurve->attach(ui->qwtAmplitudePlot);
    PhaseCurve->attach(ui->qwtPhasePlot);

}

BodeDialog::~BodeDialog()
{
    delete ui;
    delete PhaseCurve;
    delete AmplitudeCurve;
    delete PhaseMarker;
    delete AmplitudeMarker;
}

void BodeDialog::replot(double StartFrequency, double EndFrequency)
{
    /*Value  InVal(StartFrequency);
    Variable Omega(&InVal);

    double Increment = (EndFrequency - StartFrequency)/10000;

    int NumDomains = 0;
    if((StartFrequency < 1) && (EndFrequency >= 1))
        NumDomains = (int)log10(EndFrequency) - (int)log10(StartFrequency);

    if((StartFrequency >= 1) && (EndFrequency >= 1))
        NumDomains = (int)log10(EndFrequency) - (int)log10(StartFrequency);

    if((StartFrequency < 1) && (EndFrequency < 1))
        NumDomains = (int)log10(EndFrequency) - (int)log10(StartFrequency);


    if(NumDomains == 0)
        NumDomains = 1;
    int DomainPoints = 10000/NumDomains;


    try
    {
        AmplitudeMarker->setYValue(1);
        AmplitudeMarker->attach( ui->qwtAmplitudePlot );
        PhaseMarker->attach(ui->qwtPhasePlot);

        Expression->DefineVar("w", &Omega);
        int DomainCounter = log10(Omega.GetFloat());
        Value  IncrementVal(pow(10, DomainCounter));
        Variable IncrementVar(&IncrementVal);

        for(int i = 0; i < 10000 ; i++)
        {
            if((i%DomainPoints) == 0)
            {
                double DomainStart = pow(10, DomainCounter);
                double DomainEnd = pow(10, DomainCounter+1);
                double Increment = (DomainEnd - DomainStart)/DomainPoints;
                IncrementVal = Value(Increment);
                DomainCounter++;
            }

            Value Result = Expression->Eval();
            Amplitude[i] = sqrt(pow(Result.GetFloat(), 2.0) + pow(Result.GetImag(),2.0));
            Phase[i] = atan(Result.GetImag()/Result.GetFloat());
            if(Result.GetFloat() < 0.0)
                Phase[i]-=3.141/2;
            if(Result.GetImag() < 0.0)
                Phase[i]-=3.141/2;
            Frequency[i] = Omega.GetFloat();

            Omega += IncrementVar;
            if((Amplitude[i-1] >= 1.0) && (Amplitude[i] <= 1.0))
            {
                AmplitudeMarker->setXValue(Omega.GetFloat());
                QString MarkerLabelText;
                MarkerLabelText.sprintf("nyquist frequenz %f", Frequency[i]);
                AmplitudeMarker->setLabel(QwtText(MarkerLabelText));

                PhaseMarker->setYValue(Phase[i]);
                PhaseMarker->setXValue(Omega.GetFloat());
                MarkerLabelText.sprintf("Phasereserve: %f", 3.141+Phase[i]);
                PhaseMarker->setLabel(QwtText(MarkerLabelText));
            }

        }
    }
    catch(mup::ParserError &e)
    {
        std::cout << e.GetMsg() << std::endl;
    }

    setAmplitude((const double*)Frequency, (const double*)Amplitude, 10000);
    setPhase((const double*)Frequency, (const double*)Phase, 10000);

    ui->qwtAmplitudePlot->replot();
    ui->qwtPhasePlot->replot();*/
}

void BodeDialog::setAmplitude(const double *XData, const double *YData, int Size)
{
    AmplitudeCurve->setData((const double*)XData, (const double*)YData, Size);
}

void BodeDialog::setPhase(const double *XData, const double *YData, int Size)
{
    PhaseCurve->setData((const double*)XData,(const double*) YData, Size);
}

void BodeDialog::setExpression(ParserX *ExpressionA)
{
    Expression = ExpressionA;
}




