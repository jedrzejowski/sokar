#include "_base.h"

#include <gdcmFile.h>

namespace Sokar {


	class PatientDataIndicator : public SceneIndicator {
	private:
		QGraphicsTextItem* text;
		void initData();
	public:
		PatientDataIndicator(SokarDicom::DataConverter& dataConverter);
		void reposition() override;
		bool isAlive() override;
	};
}
