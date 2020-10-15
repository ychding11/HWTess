#ifndef  COMMON_DATA_STRUCTURE
#define  COMMON_DATA_STRUCTURE

#ifdef __cplusplus
#include <DirectXMath.h>
#endif


#ifdef __cplusplus
enum DiagType
{
    eDiagNormal       = 0,
    eDiagTangent      = 1,
    eDiagBiTangent    = 2,
    eDiagPosition     = 3,
    eDiagNum
};
#else
    static const int eDiagNormal       = 0;
    static const int eDiagTangent      = 1;
    static const int eDiagBiTangent    = 2;
    static const int eDiagPosition     = 3;
#endif

#ifdef __cplusplus
struct FrameParam
#else
cbuffer FrameParam : register( b0 )
#endif
{
#ifdef __cplusplus
    DirectX::XMFLOAT4X4  cbWorld;
    DirectX::XMFLOAT4X4  cbViewProjection;
    DirectX::XMFLOAT3    cbCameraPosWorld;
#else
    matrix cbWorld; // column_major is default.
    matrix cbViewProjection;
    float3 cbCameraPosWorld;
#endif
    float cbTessellationFactor;
    int   cbWireframeOn;
    int   cbHeightMapOn;
    int   cbDiagType;
    float cbTexelCellU;
    float cbTexelCellV;
    float cbWorldCell;
};

#endif  