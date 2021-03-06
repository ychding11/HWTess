#ifndef  BEZIER_SURFACE_H
#define  BEZIER_SURFACE_H

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "IDataSource.h"
#include "ShaderContainer.h"
#include "CommonType.h"


//using namespace DirectX;

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = nullptr; } }
#endif


#if 0
enum DiagType
{
    eDiagNormal       = 0,
    eDiagTangent      = 1,
    eDiagBiTangent    = 2,
    eDiagPosition     = 3,
    eDiagNum
};
struct FrameParam
{
    DirectX::XMFLOAT4X4  cbWorld;
    DirectX::XMFLOAT4X4  cbViewProjection;
    DirectX::XMFLOAT3    cbCameraPosWorld;
    float cbTessellationFactor;
    int   cbWireframeOn;
    int   cbHeightMapOn;
    int   cbDiagType;
    float cbTexelCellU;
    float cbTexelCellV;
    float cbWorldCell;
};
#endif

struct RenderOption
{
    bool wireframeOn;
    bool diagModeOn;
    bool fixedCamera;
    int heightMapOn;
    int  tessellateFactor;
    DiagType diagType;
    DirectX::XMMATRIX  world;

    RenderOption::RenderOption()
        : wireframeOn(false)
        , diagModeOn(false)
        , fixedCamera(false)
        , heightMapOn(1)
        , tessellateFactor(20)
        , diagType(eDiagNormal)
    { }

    static RenderOption& RenderOptions();

};

// Manage D3D11 resources of Tessellation surface
// Manage geometry data of Tessellation surface
// Geometry data : Tessellation Surface == 1 : 1
// Provide abstract interface for rendering
class TessSurface
{
public:

    struct ControlPoint
    {
        float controlPoint[3];
    };

protected:

    IDataSource*  mMeshData;

    ID3D11Buffer*             mpcbFrameParam = nullptr;
    ID3D11Buffer*             mpcbMaterial = nullptr;

    ID3D11Buffer*             mpControlPointVB = nullptr;
    ID3D11Buffer*             mpControlPointIB = nullptr;

    ID3D11RasterizerState*    mpRSSolid = nullptr;
    ID3D11RasterizerState*    mpRSWireframe = nullptr;

    ID3D11BlendState*         mpBSalphaToCoverage = nullptr;
    ID3D11BlendState*         mpBStransparent = nullptr;

    ID3D11SamplerState*        mpSamplerLinear = nullptr;

    ID3D11ShaderResourceView*  mpHeightMapSRV  = nullptr;
    ID3D11ShaderResourceView*  mpEnvMapSRV  = nullptr;
    ID3D11ShaderResourceView*  mpSkyMapSRV  = nullptr;

public:
    void DestroyD3D11Objects()
    {
        SAFE_RELEASE( mpcbFrameParam );

        SAFE_RELEASE( mpControlPointIB );
        SAFE_RELEASE( mpControlPointVB );

        SAFE_RELEASE( mpRSWireframe );
        SAFE_RELEASE( mpRSSolid );

        SAFE_RELEASE( mpSamplerLinear  );

        SAFE_RELEASE( mpBSalphaToCoverage );
        SAFE_RELEASE( mpBStransparent );

        SAFE_RELEASE( mpHeightMapSRV   );
        SAFE_RELEASE( mpEnvMapSRV );
        SAFE_RELEASE( mpSkyMapSRV );

    }

public:
    TessSurface(IDataSource *data = nullptr)
        : mMeshData(data) 
    { }

    virtual ~TessSurface()
    { }


    virtual void Initialize(ID3D11Device*  d3dDevice )
    {
        CreateD3D11GraphicsObjects(d3dDevice);
    }

    virtual void Render(ID3D11DeviceContext* pd3dImmediateContext) = 0;

private:
    HRESULT CreateD3D11GraphicsObjects(ID3D11Device*  d3dDevice);

protected:
    void UpdateCBParam(ID3D11DeviceContext* pd3dImmediateContex);

};

class TessQuad : public TessSurface
{
public:
    TessQuad(IDataSource *data );

    virtual void Render(ID3D11DeviceContext* pd3dImmediateContext) override;
};

class TessBezier : public TessSurface
{
public:
    TessBezier (IDataSource *data );

    virtual void Render(ID3D11DeviceContext* pd3dImmediateContext) override;
};

class TessSurfaceManager
{
public:
    static TessSurface& getTessSurface(std::string name = "");
};

#endif  