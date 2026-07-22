#include "SceneController.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

SceneController::SceneController(
    QGraphicsScene* scene
)
    :
    QObject(scene),
    scene(scene)
{
    if (!scene)
    {
        return;
    }


    scene->installEventFilter(
        this
    );
}


bool
SceneController::eventFilter(
    QObject* watched,
    QEvent* event
)
{
    if (
        watched == scene &&
        event->type() ==
            QEvent::GraphicsSceneMousePress
    )
    {
        auto* mouseEvent =
            static_cast<
                QGraphicsSceneMouseEvent*
            >(
                event
            );


        const QPointF scenePosition =
            mouseEvent->scenePos();


        emit sceneClicked(
            scenePosition.x(),
            scenePosition.y()
        );


        return true;
    }


    return QObject::eventFilter(
        watched,
        event
    );
}
