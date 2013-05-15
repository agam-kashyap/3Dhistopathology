/** \TransferFunctionEditorControlPoint class
 *         A point in the TransferFunctionEditor
 *
 *         Overloaded QGraphicsItem used in TransferFunctionEditor. Position of control points are used to
 *         calulate the TransferFunction Image values, currently has one parameter
 */
#ifndef IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H
#define IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {

    class IVW_QTEDITOR_API TransferFunctionEditorControlPoint : public QGraphicsItem {

    public:
        /** \TransferFunctionEditorControlPoint constructor  
        *      Creates a TransferFunctionEditorControlPoint at a QPointF
        */        
        TransferFunctionEditorControlPoint(TransferFunctionDataPoint* datapoint);
        
        /** \TransferFunctionEditorControlPoint constructor  
        *      Default constructor
        */
        TransferFunctionEditorControlPoint();

        /** \TransferFunctionEditorControlPoint destructor  
        */
        ~TransferFunctionEditorControlPoint();

        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType };
        int type() const  {return Type; }

        /**Paint method
        * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
        */
        virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;

        TransferFunctionDataPoint* getPoint();
        /**
        * get/set functions for id and position
        */
        const int getId();
        void setId(int id);

    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    private:
        int id_; ///< id to identify edge points
        int size_; ///< size for drawing the points
        TransferFunctionDataPoint* datapoint_; ///<The TransferFunctionDataPoint the controlpoint gets all its data from
    };

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H