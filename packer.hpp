#ifndef PACKER_HPP
#define PACKER_HPP

#include <qobject.h>
#include <QRunnable>
#include "collection.hpp"

class Packer : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Packer(Collection _collection, QObject* parent = Q_NULLPTR);
    void pack(Collection collection);
    void run() override;
    Collection collection;
};

#endif
