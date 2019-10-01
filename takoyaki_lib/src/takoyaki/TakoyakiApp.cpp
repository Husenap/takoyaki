#include "TakoyakiApp.h"

#include "editor/ServiceManager.h"

namespace ty {
TakoyakiApp::TakoyakiApp() {
	ty::ServiceManager serviceManager;
}
TakoyakiApp::~TakoyakiApp() {}
}  // namespace ty