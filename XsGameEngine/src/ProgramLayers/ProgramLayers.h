#pragma once


//#define IM_GUI_LAYER


#pragma region Program Layers
#define GUI_LAYER	//Game Engine GUI Layer

//CHANGE BEFORE LAUNCH
#ifdef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = true;
#endif

#ifdef IM_GUI_LAYER
	//#define GUI_LAYER
// IM_GUI_LAYER
#endif
#pragma endregion