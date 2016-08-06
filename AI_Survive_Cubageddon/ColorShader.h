#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class ColorShader
{
// Member Level Variables
private:
	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11Buffer* _matrixBuffer;

// Function Declarations
private:
	bool InitialiseShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);
public:
	ColorShader();
	ColorShader(const ColorShader&);
	~ColorShader();

	bool Initialise(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);
};

#endif