#include "util.h"

bool WorldToScreen(vec3 pos, vec3& screen, float matrix[16], int windowWidth, int windowHeight){
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4 clipCoords;

	clipCoords.w = pos.v[POS_FORWARD] * matrix[3] + pos.v[POS_VERTICAL] * matrix[7] + pos.v[POS_STRAFE] * matrix[11] + matrix[15];

	if (clipCoords.w < 0.65f)
		return false;

	clipCoords.x = pos.v[POS_FORWARD] * matrix[0] + pos.v[POS_VERTICAL] * matrix[4] + pos.v[POS_STRAFE] * matrix[8] + matrix[12];
	clipCoords.y = pos.v[POS_FORWARD] * matrix[1] + pos.v[POS_VERTICAL] * matrix[5] + pos.v[POS_STRAFE] * matrix[9] + matrix[13];

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;

	//convert NDC to screen coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	screen.z = 0;

	return true;
}

//Both W2S functions produce the same output
bool WorldToScreen2(vec3 pos, vec3& screen, float* matrix, int windowWidth, int windowHeight){
	float matrix2[4][4];

	memcpy(matrix2, matrix, 16 * sizeof(float));

	float mX = windowWidth / 2;
	float mY = windowHeight / 2;

	float w =
		matrix2[0][3] * pos.x +
		matrix2[1][3] * pos.y +
		matrix2[2][3] * pos.z +
		matrix2[3][3];

	if (w < 0.65f)
		return false;

	float x =
		matrix2[0][0] * pos.x +
		matrix2[1][0] * pos.y +
		matrix2[2][0] * pos.z +
		matrix2[3][0];

	float y =
		matrix2[0][1] * pos.x +
		matrix2[1][1] * pos.y +
		matrix2[2][1] * pos.z +
		matrix2[3][1];

	screen.x = (mX + mX * x / w);
	screen.y = (mY - mY * y / w);
	screen.z = 0;

	return true;
}

std::string UTF16ToUTF8(std::wstring ws){
    // https://docs.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
    int wssize = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, NULL, 0, 0, NULL);
    char buffer[wssize+1] = {0};
    WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, buffer, wssize, 0, NULL);
    return std::string(buffer);
}
