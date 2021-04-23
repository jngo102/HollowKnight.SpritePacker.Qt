#ifndef STDAFX_HPP
#define STDAFX_HPP

#if defined(_WIN32)
	#define ROOT_SPRITES_DIRECTORY   QDir::home().filePath("AppData/LocalLow/Team Cherry/Hollow Knight/sprites")
#elif defined(unix)
    #if defined(ANDROID)
        #define ROOT_SPRITES_DIRECTORY   QStandardPaths::locate(QStandardPaths::PicturesLocation, "sprites", QStandardPaths::LocateDirectory)
    #else
        #define ROOT_SPRITES_DIRECTORY   QDir::home().filePath(".config/unity3d/Team Cherry/Hollow Knight/sprites")
    #endif
#elif defined(__APPLE__)
    #define ROOT_SPRITES_DIRECTORY   QDir::home().filePath("Library/Application Support/unity.Team Cherry.Hollow Knight/sprites")
#else
	#pragma warning("Your machine is not compatible with this program!")
#endif

#include <QtWidgets>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <cstdlib>
#include <fstream>

#endif
