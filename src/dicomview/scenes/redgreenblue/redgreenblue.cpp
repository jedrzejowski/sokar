#include <src/sokar/gdcmSokar.h>
#include "redgreenblue.h"

using namespace Sokar;

RedGreenBlue::Scene::Scene(SceneParams &sceneParams) :
		DicomScene(sceneParams) {

	reloadPixmap();
}

bool RedGreenBlue::Scene::generatePixmap() {
	if (!pixmap.isNull()) return false;

	qDebug() << "regen";
	gdcm::assertTagPresence(gdcmDataSet, gdcm::TagPlanarConfiguration);

	auto planarConfiguration = (ushort) *(gdcmDataSet.GetDataElement(
			gdcm::TagPlanarConfiguration).GetByteValue()->GetPointer());

	switch (gdcmImage.GetPixelFormat()) {
		case gdcm::PixelFormat::UINT8: {

			auto *origin = (quint8 *) &originBuffer[0];
			auto *buffer = &targetBuffer[0];
			quint64 nextOffset = 0, redOffset = 0, greenOffset = 0, blueOffset = 0;

			if (planarConfiguration == 0) {
				nextOffset = 3;
				redOffset = 0;
				greenOffset = 1;
				blueOffset = 2;
			} else if (planarConfiguration == 1) {
				nextOffset = 1;
				redOffset = 0;
				greenOffset = 1 * imgDimX * imgDimY;
				blueOffset = 2 * imgDimX * imgDimY;
			} else throw Sokar::ImageTypeNotSupportedException();

			for (quint64 i = 0; i < imgDimX * imgDimY; i++) {
				*buffer = Pixel(
						*(origin + redOffset),
						*(origin + greenOffset),
						*(origin + blueOffset)
				);

				buffer++;
				origin += nextOffset;
			}
		}
			break;

		case gdcm::PixelFormat::UINT16:
			qDebug("Szach Mat xD, mam taki plik");
			throw Sokar::ImageTypeNotSupportedException();

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}

	auto img = QImage((uchar *) &targetBuffer[0], imgDimX, imgDimY, sizeof(Pixel) * imgDimX, QImage::Format_RGB888);
	pixmap.convertFromImage(img);

	return true;
}

void RedGreenBlue::Scene::toolBarAdjust(DicomToolBar *toolbar) {
	DicomScene::toolBarAdjust(toolbar);

	toolbar->getActionWindowing()->setDisabled(true);
}
