#include "paths.h"
#include <QStandardPaths>
#include <QCoreApplication>
#include <QString>
#include <QFileInfo>
#include <easylogging++.h>
#include <QDir>

namespace paths
{
	optional<string> binaryDirectory()
	{
		const auto path = QCoreApplication::applicationDirPath();

		if (path == "")
		{
			LOG(ERROR) << u8"バイナリディレクトリが見つかりませんでした！";
			return std::nullopt;
		}

		return path.toStdString();
	}

	optional<string> binaryDirectoryFindFile(const string fileName)
	{
		const auto path = binaryDirectory();

		if (!path)
		{
			return std::nullopt;
		}

		const auto filePath = QDir(QString::fromStdString(*path) + '/'
			+ QString::fromStdString(fileName));
		QFileInfo file(filePath.path());

		if (!file.exists())
		{
			LOG(ERROR) << u8"ファイルが見つかりませんでした：「" << fileName.c_str()
				<< u8"」、バイナリディレクトリで！";
			return std::nullopt;
		}

		return QDir::toNativeSeparators(file.filePath()).toStdString();
	}

	optional<string> settingsDirectory()
	{
		const auto path
			= QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		if (path == "")
		{
			LOG(ERROR) << u8"セッティングディレクトリが見つかりませんでした！";
			return std::nullopt;
		}

		return path.toStdString();
	}

	std::optional<std::string> verifyIconFilePath(std::string filename)
	{
		const auto notifIconPath = paths::binaryDirectoryFindFile(filename);
		if (!notifIconPath.has_value())
		{
			LOG(ERROR) << u8"アイコンが見つかりませんでした：「" << filename << "」";
		}

		return notifIconPath;
	}

} // namespace paths
