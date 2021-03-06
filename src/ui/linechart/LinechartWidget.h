/*=====================================================================

PIXHAWK Micro Air Vehicle Flying Robotics Toolkit

(c) 2009 PIXHAWK PROJECT  <http://pixhawk.ethz.ch>

This file is part of the PIXHAWK project

    PIXHAWK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PIXHAWK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PIXHAWK. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Definition of Line chart plot widget
 *
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */
#ifndef LINECHARTWIDGET_H
#define LINECHARTWIDGET_H

#include <QGridLayout>
#include <QWidget>
#include <QFrame>
#include <QComboBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QScrollBar>
#include <QSpinBox>
#include <QMap>
#include <QString>
#include <QAction>
#include <QIcon>
#include <QLabel>
#include <QReadWriteLock>
#include <QToolButton>
#include <QTimer>
#include <qwt_plot_curve.h>

#include "LinechartPlot.h"
#include "UASInterface.h"
#include "ui_Linechart.h"

#include "LogCompressor.h"

/**
 * @brief The linechart widget allows to visualize different timeseries as lineplot.
 * The display interval, the timeseries and the scaling can be changed interactively
 **/
class LinechartWidget : public QWidget {
    Q_OBJECT

public:
    LinechartWidget(int systemid, QWidget *parent = 0);
    ~LinechartWidget();

    static const int MIN_TIME_SCROLLBAR_VALUE = 0; ///< The minimum scrollbar value
    static const int MAX_TIME_SCROLLBAR_VALUE = 16383; ///< The maximum scrollbar value

public slots:
    void addCurve(QString curve);
    void removeCurve(QString curve);
    void appendData(int uasId, QString curve, double data, quint64 usec);
    void takeButtonClick(bool checked);
    void setPlotWindowPosition(int scrollBarValue);
    void setPlotWindowPosition(quint64 position);
    void setPlotInterval(quint64 interval);
    /** @brief Start automatic updates once visible */
    void showEvent(QShowEvent* event);
    /** @brief Stop automatic updates once hidden */
    void hideEvent(QHideEvent* event);
    void setActive(bool active);
    /** @brief Set the number of values to average over */
    void setAverageWindow(int windowSize);
    /** @brief Start logging to file */
    void startLogging();
    /** @brief Stop logging to file */
    void stopLogging();
    /** @brief Refresh the view */
    void refresh();

protected:
    void addCurveToList(QString curve);
    void removeCurveFromList(QString curve);
    QToolButton* createButton(QWidget* parent);
    QWidget* createCurveItem(QString curve);
    void createLayout();

    int sysid;                            ///< ID of the unmanned system this plot belongs to
    LinechartPlot* activePlot;            ///< Plot for this system
    QReadWriteLock* curvesLock;           ///< A lock (mutex) for the concurrent access on the curves
    QReadWriteLock plotWindowLock;        ///< A lock (mutex) for the concurrent access on the window position

    int curveListIndex;
    int curveListCounter;                 ///< Counter of curves in curve list
    QList<QString>* listedCurves;         ///< Curves listed
    QMap<QString, QLabel*>* curveLabels;  ///< References to the curve labels
    QMap<QString, QLabel*>* curveMeans;   ///< References to the curve means
    QMap<QString, QLabel*>* curveMedians; ///< References to the curve medians
    QMap<QString, QLabel*>* curveVariances; ///< References to the curve variances

    QWidget* curvesWidget;                ///< The QWidget containing the curve selection button
    QVBoxLayout* curvesWidgetLayout;      ///< The layout for the curvesWidget QWidget
    QScrollBar* scrollbar;                ///< The plot window scroll bar
    QSpinBox* averageSpinBox;             ///< Spin box to setup average window filter size

    QAction* setScalingLogarithmic;       ///< Set logarithmic scaling
    QAction* setScalingLinear;            ///< Set linear scaling
    QAction* addNewCurve;                 ///< Add curve candidate to the active curves

    QMenu* curveMenu;
    QGridLayout* mainLayout;

    QToolButton* scalingLinearButton;
    QToolButton* scalingLogButton;
    QToolButton* logButton;

    QFile* logFile;
    unsigned int logindex;
    bool logging;
    QTimer* updateTimer;
    LogCompressor* compressor;
    quint64 logStartTime;

    static const int MAX_CURVE_MENUITEM_NUMBER = 8;
    static const int PAGESTEP_TIME_SCROLLBAR_VALUE = (MAX_TIME_SCROLLBAR_VALUE - MIN_TIME_SCROLLBAR_VALUE) / 10;

private:
    Ui::linechart ui;
    void createActions();

signals:
    /**
         * @brief This signal is emitted if a curve is removed from the list
         *
         * @param curve The removed plot curve
         **/
    void curveRemoved(QString curve);

    /**
         * @brief This signal is emitted if a curve has been moved or added
         *
         * @param curve The moved or added curve
         * @param position The x-position of the curve (The centerline)
         **/
    void curveSet(QString curve, int position);

    /**
         * @brief This signal is emitted to change the visibility of a curve
         *
         * @param curve The changed curve
         * @pram visible The visibility
         **/
    void curveVisible(QString curve, bool visible);

    void plotWindowPositionUpdated(quint64 position);
    void plotWindowPositionUpdated(int position);

    /** @brief This signal is emitted once a logfile has been finished writing */
    void logfileWritten(QString fileName);

};

#endif // LINECHARTWIDGET_H
