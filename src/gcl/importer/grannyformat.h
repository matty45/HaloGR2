#pragma once

#include <Windows.h>
#include <fstream>

using namespace std;

// Granny specific data structures.

typedef float GrannyTriple[3];
typedef float GrannyQuad[4];
typedef float GrannyMatrix4x4[4][4];

///
/// \brief Stores data of pixel layout.
///
struct GrannyPixelLayout {
	int BytesPerPixel;
	int ShiftForComponent[4];
	int BitsForComponent[4];
};

///
/// \brief Stores of data of mip level of a texture image.
///
struct GrannyTextureMIPLevel {
	int Stride;
	int PixelByteCount;
	void* PixelBytes;
};

///
/// \brief Stores details for a texture image.
///
struct GrannyTextureImage {
	int MIPLevelCount;
	GrannyTextureMIPLevel* MIPLevels;
};

///
/// \brief Stores a variant of data structure.
///
struct GrannyVariant {
	void* Type;
	void* Object;
};

///
/// \brief Member types for custom data type definitions.
///
enum GrannyMemberType {
	GrannyEndMember,
	GrannyInlineMember,
	GrannyReferenceMember,
	GrannyReferenceToArrayMember,
	GrannyArrayOfReferencesMember,
	GrannyVariantReferenceMember,
	GrannyUnsupportedMemberType_Remove,
	GrannyReferenceToVariantArrayMember,
	GrannyStringMember,
	GrannyTransformMember,
	GrannyReal32Member,
	GrannyInt8Member,
	GrannyUInt8Member,
	GrannyBinormalInt8Member,
	GrannyNormalUInt8Member,
	GrannyInt16Member,
	GrannyUInt16Member,
	GrannyBinormalInt16Member,
	GrannyNormalUInt16Member,
	GrannyInt32Member,
	GrannyUInt32Member,
	GrannyReal16Member,
	GrannyEmptyReferenceMember,
	GrannyOnePastLastMemberType,
	GrannyBool32Member = GrannyInt32Member,
	GrannyMemberTypeForceInt = 0x7fffffff
};

///
/// \brief Defines a custom data type .e.g. for extensions.
///
struct GrannyDataTypeDefinition {
	GrannyMemberType Type;
	char const* Name = "";
	GrannyDataTypeDefinition* ReferenceType = nullptr;
	int ArrayWidth = 0;
	int Extra[3] = { 0, 0, 0 };
	void* Ignored_Ignored = nullptr;
};

///
/// \brief Stores all data of a texture.
///
#pragma pack(push,1)
struct GrannyTexture {
	char const* FromFileName;
	int TextureType;
	int Width;
	int Height;
	int Encoding;
	int SubFormat;
	GrannyPixelLayout Layout;
	int ImageCount;
	GrannyTextureImage* Images;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyTexture) == 0x5c);

struct GrannyMaterialMap;

///
/// \brief Stores all data of a material.
///
#pragma pack(push,1)
struct GrannyMaterial {
	char const* Name;
	int MapCount;
	GrannyMaterialMap* Maps;
	GrannyTexture* Texture;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyMaterial) == 0x2c);


///
/// \brief Stores a material map.
///
struct GrannyMaterialMap {
	char const* Usage;
	GrannyMaterial* Material;
};

static_assert(sizeof(GrannyMaterialMap) == 0x10);

///
/// \brief Stores a reference to a material.
///
struct GrannyMaterialBinding {
	GrannyMaterial* Material;
};

static_assert(sizeof(GrannyMaterialBinding) == 0x8);

///
/// \brief Stores all transform data of a transformation.
///
struct GrannyTransform {
	unsigned int Flags;
	float Position[3];
	float Orientation[4];
	float ScaleShear[3][3];
};

static_assert(sizeof(GrannyTransform) == 0x44);

///
/// \brief Stores a variant of animation curve.
///
struct GrannyCurve2 {
	GrannyVariant CurveData;
};

static_assert(sizeof(GrannyCurve2) == 0x10);

///
/// \brief Stores all data of an animation vector track.
///
#pragma pack(push,1)
struct GrannyVectorTrack {
	char const* Name;
	unsigned int TrackKey;
	int Dimension;
	GrannyCurve2 ValueCurve;
};
#pragma pack(pop)
static_assert(sizeof(GrannyVectorTrack) == 0x20);

#pragma pack(push,1)
struct GrannyTextTrackEntry
{
	float TimeStamp;
	char* Text;
};
#pragma pack(pop)

static_assert(sizeof(GrannyTextTrackEntry) == 0xc);

///
/// \brief Stores all data of an animation text track.
///
#pragma pack(push,1)
struct GrannyTextTrack {
	char const* Name;
	int EntryCount;
	GrannyTextTrackEntry* Entries;
};
#pragma pack(pop)
static_assert(sizeof(GrannyTextTrack) == 0x14);

///
/// \brief Stores all data of an animation transform track.
///
#pragma pack(push,1)
struct GrannyTransformTrack {
	char const* Name;
	int Flags;
	GrannyCurve2 OrientationCurve;
	GrannyCurve2 PositionCurve;
	GrannyCurve2 ScaleShearCurve;
};
#pragma pack(pop)
static_assert(sizeof(GrannyTransformTrack) == 0x3c);

struct GrannyPeriodicLoop
{
	float Radius;
	float dAngle;
	float dZ;
	float BasisX[3];
	float BasisY[3];
	float Axis[3];
};
static_assert(sizeof(GrannyPeriodicLoop) == 0x30);

///
/// \brief Stores all data of an animation track group.
///
#pragma pack(push,1)
struct GrannyTrackGroup {
	char* Name;
	int VectorTrackCount;
	GrannyVectorTrack* VectorTracks;
	int TransformTrackCount;
	GrannyTransformTrack* TransformTracks;
	int TransformLODErrorCount;
	float* TransformLODErrors;
	int TextTrackCount;
	GrannyTextTrack* TextTracks;
	GrannyTransform InitialPlacement;
	int Flags;
	float LoopTranslation[3];
	GrannyPeriodicLoop* PeriodicLoop;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyTrackGroup) == 0xa4);

///
/// \brief Stores all data of a bone binding.
///
#pragma pack(push,1)
struct GrannyBoneBinding {
	char const* BoneName;
	GrannyTriple OBBMin;
	GrannyTriple OBBMax;
	int TriangleCount;
	int* TriangleIndices;
};
#pragma pack(pop)
static_assert(sizeof(GrannyBoneBinding) == 0x2c);

///
/// \brief Stores all data of a bone.
///
#pragma pack(push,1)
struct GrannyBone {
	char const* Name;
	int ParentIndex;
	GrannyTransform LocalTransform;
	GrannyMatrix4x4 InverseWorld4x4;
	float LODError;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyBone) == 0xa4);

///
/// \brief Stores all data of a skeleton.
///
#pragma pack(push,1)
struct GrannySkeleton {
	char const* Name;
	int BoneCount;
	GrannyBone* Bones;
	int LODType;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannySkeleton) == 0x28);

///
/// \brief Stores all data of a triangle annotation set.
///
#pragma pack(push,1)
struct GrannyTriAnnotationSet {
	char const* Name;
	GrannyDataTypeDefinition* TriAnnotationType;
	int TriAnnotationCount;
	unsigned char* TriAnnotations;
	int IndicesMapFromTriToAnnotation;
	int TriAnnotationIndexCount;
	int* TriAnnotationIndices;
};
#pragma pack(pop)
static_assert(sizeof(GrannyTriAnnotationSet) == 0x2c);

///
/// \brief Stores all data of a triangle group.
///
struct GrannyTriMaterialGroup {
	int MaterialIndex;
	int TriFirst;
	int TriCount;
};

static_assert(sizeof(GrannyTriMaterialGroup) == 0xc);

///
/// \brief Stores all data of a triangle.
///
#pragma pack(push,1)
struct GrannyTriTopology {
	int GroupCount;
	GrannyTriMaterialGroup* Groups;
	int IndexCount;
	int* Indices;
	int Index16Count;
	unsigned short* Indices16;
	int VertexToVertexCount;
	int* VertexToVertexMap;
	int VertexToTriangleCount;
	int* VertexToTriangleMap;
	int SideToNeighborCount;
	unsigned int* SideToNeighborMap;
	int BonesForTriangleCount;
	int* BonesForTriangle;
	int TriangleToBoneCount;
	int* TriangleToBoneIndices;
	int TriAnnotationSetCount;
	GrannyTriAnnotationSet* TriAnnotationSets;
};
#pragma pack(pop)
static_assert(sizeof(GrannyTriTopology) == 0x6c);

///
/// \brief Stores all data of a vertex annotation set.
///
#pragma pack(push,1)
struct GrannyVertexAnnotationSet {
	char const* Name;
	GrannyDataTypeDefinition* VertexAnnotationType;
	int VertexAnnotationCount;
	unsigned char* VertexAnnotations;
	int IndicesMapFromVertexToAnnotation;
	int VertexAnnotationIndexCount;
	int* VertexAnnotationIndices;
};
#pragma pack(pop)
static_assert(sizeof(GrannyVertexAnnotationSet) == 0x2c);

///
/// \brief Stores all vertex data of a mesh.
///
#pragma pack(push,1)
struct GrannyVertexData {
	GrannyDataTypeDefinition* VertexType;
	int VertexCount;
	unsigned char* Vertices;
	int VertexComponentNameCount;
	char** VertexComponentNames;
	int VertexAnnotationSetCount;
	GrannyVertexAnnotationSet* VertexAnnotationSets;
};
#pragma pack(pop)
static_assert(sizeof(GrannyVertexData) == 0x2c);

///
/// \brief Stores all morph target data of a mesh.
///
#pragma pack(push,1)
struct GrannyMorphTarget {
	char* ScalarName;
	GrannyVertexData* VertexData;
	int DataIsDeltas;
};
#pragma pack(pop)
static_assert(sizeof(GrannyMorphTarget) == 0x14);

///
/// \brief Stores all data of a mesh.
///
#pragma pack(push,1)
struct GrannyMesh {
	char const* Name;
	GrannyVertexData* PrimaryVertexData;
	int MorphTargetCount;
	GrannyMorphTarget* MorphTargets;
	GrannyTriTopology* PrimaryTopology;
	int MaterialBindingCount;
	GrannyMaterialBinding* MaterialBindings;
	int BoneBindingCount;
	GrannyBoneBinding* BoneBindings;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyMesh) == 0x4c);

///
/// \brief Stores a reference to a mesh also called MeshBinding.
///
struct GrannyModelMeshBinding {
	GrannyMesh* Mesh;
};

///
/// \brief Stores all data of a model.
///
struct GrannyModel {
	char const* Name;
	GrannySkeleton* Skeleton;
	GrannyTransform InitialPlacement;
	int MeshBindingCount;
	GrannyModelMeshBinding* MeshBindings;
	GrannyVariant ExtendedData;
};

static_assert(sizeof(GrannyModel) == 0x70);

///
/// \brief Stores vertex data of a variant of 4 components based vertex structure.
///
/// Meaning of PWNT:
///  P: Position
///  W: Weights
///  N: Normal
///  T: Texture (UV)
///
struct GrannyPWNT3432Vertex {
	float Position[3];
	unsigned char BoneWeights[4];
	unsigned char BoneIndices[4];
	float Normal[3];
	float UV[2];
};

#define GrannyVertexTextureCoordinatesName "TextureCoordinates"

///
/// \brief Defines a 5 components based vertex structure PWNT.
///
/// Meaning of PWNT:
///  P: Position
///  W: Weights
///  N: Normal
///  T: Texture coordinates for uv channel 1 and 2
///
static GrannyDataTypeDefinition GrannyPWNT34322VertexType[] = {
	{ GrannyReal32Member, "Position", 0, 3 },
	{ GrannyNormalUInt8Member, "BoneWeights", 0, 4 },
	{ GrannyUInt8Member, "BoneIndices", 0, 4 },
	{ GrannyReal32Member, "Normal", 0, 3 },
	{ GrannyReal32Member, GrannyVertexTextureCoordinatesName "0", 0, 2 },
	{ GrannyReal32Member, GrannyVertexTextureCoordinatesName "1", 0, 2 },
	{ GrannyEndMember },
};

///
/// \brief Stores vertex data of a variant of 5 components based vertex structure.
///
/// Meaning of PWNT:
///  P: Position
///  W: Weights
///  N: Normal
///  T: Texture coordinates for uv channel 1 (UV1) and 2 (UV2)
///
struct GrannyPWNT34322Vertex {
	float Position[3];
	unsigned char BoneWeights[4];
	unsigned char BoneIndices[4];
	float Normal[3];
	float UV1[2];
	float UV2[2];
};

///
/// \brief Stores header of an animation curve like curve format or degree elevation of a curve.
///
struct GrannyCurveDataHeader {
	unsigned char Format;
	unsigned char Degree;
};

static_assert(sizeof(GrannyCurveDataHeader) == 0x2);

///
/// \brief Stores animation curve data for variant of a keyframe based constant curve.
///
struct GrannyCurveDataDAK32fC32f {
	GrannyCurveDataHeader CurveDataHeader;
	short Padding;
	int KnotCount;
	float* Knots;
	int ControlCount;
	float* Controls;
};

///
/// \brief Stores all granny curve data formats.
///
enum GrannyCurveDataFormat {
	DaKeyframes32f = 0,
	DaK32fC32f = 1,
	DaIdentity = 2,
	DaConstant32f = 3,
	D3Constant32f = 4,
	D4Constant32f = 5,
	DaK16uC16u = 6,
	DaK8uC8u = 7,
	D4nK16uC15u = 8,
	D4nK8uC7u = 9,
	D3K16uC16u = 10,
	D3K8uC8u = 11,
	D9I1K16uC16u = 12,
	D9I3K16uC16u = 13,
	D9I1K8uC8u = 14,
	D9I3K8uC8u = 15,
	D3I1K32fC32f = 16,
	D3I1K16uC16u = 17,
	D3I1K8uC8u = 18
};

///
/// \brief Stores all data of an animation like name and tracks.
///
struct GrannyAnimation {
	char const* Name;
	float Duration;
	float TimeStep;
	float Oversampling;
	int TrackGroupCount;
	GrannyTrackGroup** TrackGroups;
	int DefaultLoopCount;
	int Flags;
	GrannyVariant ExtendedData;
};

static_assert(sizeof(GrannyAnimation) == 0x38);

#define GrannyNoParentBone -1

///
/// \brief Transform flags
///
enum GrannyTransformFlags {
	GrannyHasPosition = 0x1,
	GrannyHasOrientation = 0x2,
	GrannyHasScaleShear = 0x4,
	GrannyTransformFlagsForceint = 0x7fffffff
};

///
/// \brief Stores section indexes and their offsets.
///
struct GrannyRef
{
	unsigned int SectionIndex;
	unsigned int Offset;
};

///
/// \brief Stores all header data of a granny file.
///
struct GrannyFileHeader {
	unsigned int Version;
	unsigned int TotalSize;
	unsigned int CRC;
	unsigned int SectionArrayOffset;
	unsigned int SectionArrayCount;
	GrannyRef RootObjectTypeDefinition;
	GrannyRef RootObject;
	unsigned int TypeTag;
	unsigned int ExtraTags[4];
	unsigned int StringDatabaseCRC;
	unsigned int ReservedUnused[3];
};

static_assert(sizeof(GrannyFileHeader) == 0x48);

///
/// \brief Stores all header magic data of a granny file.
///
struct GrannyFileMagic {
	unsigned int MagicValue[4];
	unsigned int HeaderSize;
	unsigned int HeaderFormat;
	unsigned int Reserved[2];
};

static_assert(sizeof(GrannyFileMagic) == 0x20);

///
/// \brief Stores all header and data blocks of a granny file.
///
#pragma pack(push,1)
struct GrannyFile {
	int IsByteReversed;
	GrannyFileHeader* Header;
	GrannyFileMagic* SourceMagicValue;
	int SectionCount;
	void** Sections;
	bool* Marshalled;
	bool* IsUserMemory;
	void* ConversionBuffer;
};
#pragma pack(pop)
static_assert(sizeof(GrannyFile) == 0x38);

///
/// \brief Stores data related to the tool used to make the granny file.
///
#pragma pack(push,1)
struct GrannyArtToolInfo {
	char const* FromArtToolName;
	int ArtToolMajorRevision;
	int ArtToolMinorRevision;
	int ArtToolPointerSize;
	float UnitsPerMeter;
	float Origin[3];
	float RightVector[3];
	float UpVector[3];
	float BackVector[3];
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyArtToolInfo) == 0x58);

///
/// \brief Stores data related to the exporter script/tool/whatever used to export the granny file.
///
#pragma pack(push,1)
struct GrannyExporterInfo {
	char* ExporterName;
	int ExporterMajorRevision;
	int ExporterMinorRevision;
	int ExporterCustomization;
	int ExporterBuildNumber;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyExporterInfo) == 0x28);

///
/// \brief Stores all raw data of a granny file.
///
#pragma pack(push,1)
struct GrannyFileInfo {
	GrannyArtToolInfo* ArtToolInfo;
	GrannyExporterInfo* ExporterInfo;
	char* FromFileName;
	int TextureCount;
	GrannyTexture** Textures;
	int MaterialCount;
	GrannyMaterial** Materials;
	int SkeletonCount;
	GrannySkeleton** Skeletons;
	int VertexDataCount;
	GrannyVertexData** VertexDatas;
	int TriTopologyCount;
	GrannyTriTopology** TriTopologies;
	int MeshCount;
	GrannyMesh** Meshes;
	int ModelCount;
	GrannyModel** Models;
	int TrackGroupCount;
	GrannyTrackGroup** TrackGroups;
	int AnimationCount;
	GrannyAnimation** Animations;
	GrannyVariant ExtendedData;
};
#pragma pack(pop)
static_assert(sizeof(GrannyFileInfo) == 0x94);

///
/// \brief Scene transform flags.
///
enum GrannyTransformFileFlags {
	GrannyRenormalizeNormals = 0x1,
	GrannyReorderTriangleIndices = 0x2,
	Grannytransform_file_flags_forceint = 0x7fffffff
};

// Type definitions for required functions from granny2_x64.dll.

typedef GrannyFile* (__stdcall* GrannyReadEntireFile_t)(const char* FileName);
typedef GrannyFileInfo* (__stdcall* GrannyGetFileInfo_t)(GrannyFile* File);
typedef void(__stdcall* GrannyFreeFile_t)(GrannyFile const* File);
typedef int(__stdcall* GrannyGetTotalTypeSize_t)(GrannyDataTypeDefinition* TypeDefinition);
typedef int(__stdcall* GrannyGetMeshVertexCount_t)(GrannyMesh const* Mesh);
typedef void(__stdcall* GrannyCopyMeshVertices_t)(GrannyMesh const* Mesh, GrannyDataTypeDefinition const* VertexType, void* DestVertices);
typedef int(__stdcall* GrannyGetMeshIndexCount_t)(GrannyMesh const* Mesh);
typedef void(__stdcall* GrannyCopyMeshIndices_t)(GrannyMesh const* Mesh, int BytesPerIndex, void* DestIndices);
typedef void(__stdcall* GrannyBuildCompositeTransform4x4_t)(GrannyTransform const* Transform, float* Composite4x4);
typedef bool(__stdcall* GrannyMeshIsRigid_t)(GrannyMesh const* Mesh);

typedef bool(__stdcall* GrannyComputeBasisConversion_t)(
	GrannyFileInfo const* FileInfo,
	float DesiredUnitsPerMeter,
	float const* DesiredOrigin3,
	float const* DesiredRight3,
	float const* DesiredUp3,
	float const* DesiredBack3,
	float* ResultAffine3,
	float* ResultLinear3x3,
	float* ResultInverseLinear3x);

typedef void(__stdcall* GrannyTransformFile_t)(
	GrannyFileInfo* FileInfo,
	float const* Affine3,
	float const* Linear3x3,
	float const* InverseLinear3x3,
	float AffineTolerance,
	float LinearTolerance,
	unsigned Flags);

typedef void(__stdcall* GrannyCurveMakeStaticDaK32fC32f_t)(
	GrannyCurve2* Curve,
	GrannyCurveDataDAK32fC32f* CurveData,
	int KnotCount,
	int Degree,
	int Dimension,
	float const* Knots,
	float const* Controls);

typedef GrannyCurve2* (__stdcall* GrannyCurveConvertToDaK32fC32f_t)(
	GrannyCurve2 const* SrcCurve,
	float const* IdentityVector);

typedef void(__stdcall* GrannyFreeCurve_t)(GrannyCurve2* Curve);
typedef int(__stdcall* GrannyCurveGetKnotCount_t)(GrannyCurve2 const* Curve);
typedef int(__stdcall* GrannyCurveGetDimension_t)(GrannyCurve2 const* Curve);
typedef int(__stdcall* GrannyCurveGetDegree_t)(GrannyCurve2 const* Curve);
typedef float* GrannyCurveIdentityPosition_t;
typedef float* GrannyCurveIdentityOrientation_t;
typedef float* GrannyCurveIdentityScaleShear_t;
typedef float* GrannyCurveIdentityScale_t;

typedef void(__stdcall* GrannyEvaluateCurveAtT_t)(
	int Dimension,
	bool Normalize,
	bool BackwardsLoop,
	GrannyCurve2 const* Curve,
	bool ForwardsLoop,
	float CurveDuration,
	float t,
	float* Result,
	float const* IdentityVector);

typedef void(__stdcall* GrannyEvaluateCurveAtKnotIndex_t)(
	int Dimension,
	bool Normalize,
	bool BackwardsLoop,
	GrannyCurve2 const* Curve,
	bool ForwardsLoop,
	float CurveDuration,
	int KnotIndex,
	float t,
	float* Result,
	float const* IdentityVector);

typedef int(__stdcall* GrannyFindKnot_t)(
	int KnotCount,
	float* Knots,
	float t);

typedef int(__stdcall* GrannyFindCloseKnot_t)(
	int KnotCount,
	float* Knots,
	float t,
	int StartinIndex);

typedef bool(__stdcall* GrannyCurveIsKeyframed_t)(GrannyCurve2 const* Curve);
typedef void(__stdcall* GrannyCurveInitializeFormat_t)(GrannyCurve2* Curve);
typedef void(__stdcall* GrannyCurveInitializeFormat_t)(GrannyCurve2* Curve);
typedef GrannyDataTypeDefinition* GrannyCurveDataDaIdentityType_t;
typedef bool(__stdcall* GrannyTextureHasAlpha_t)(GrannyTexture const* Texture);
typedef GrannyPixelLayout* GrannyRGBA8888PixelFormat_t;
typedef GrannyPixelLayout* GrannyRGB888PixelFormat_t;

typedef void(__stdcall* GrannyCopyTextureImage_t)(
	GrannyTexture const* Texture,
	int ImageIndex,
	int MIPIndex,
	GrannyPixelLayout const* Layout,
	int DestWidth,
	int DestHeight,
	int DestStride,
	void* Pixels);

// Declarations for required functions of granny2_x64.dll.
// Declarations will get assigned by InitializeGrannyLibrary function.

inline GrannyReadEntireFile_t GrannyReadEntireFile = nullptr;
inline GrannyGetFileInfo_t GrannyGetFileInfo = nullptr;
inline GrannyFreeFile_t GrannyFreeFile = nullptr;
inline GrannyDataTypeDefinition* GrannyPWNT3432VertexType = 0;
inline GrannyGetTotalTypeSize_t GrannyGetTotalTypeSize = nullptr;
inline GrannyGetMeshVertexCount_t GrannyGetMeshVertexCount = nullptr;
inline GrannyGetMeshIndexCount_t GrannyGetMeshIndexCount = nullptr;
inline GrannyCopyMeshVertices_t GrannyCopyMeshVertices = nullptr;
inline GrannyCopyMeshIndices_t GrannyCopyMeshIndices = nullptr;
inline GrannyBuildCompositeTransform4x4_t GrannyBuildCompositeTransform4x4 = nullptr;
inline GrannyMeshIsRigid_t GrannyMeshIsRigid = nullptr;
inline GrannyComputeBasisConversion_t GrannyComputeBasisConversion = nullptr;
inline GrannyTransformFile_t GrannyTransformFile = nullptr;
inline GrannyCurveMakeStaticDaK32fC32f_t GrannyCurveMakeStaticDaK32fC32f = nullptr;
inline GrannyCurveConvertToDaK32fC32f_t GrannyCurveConvertToDaK32fC32f = nullptr;
inline GrannyFreeCurve_t GrannyFreeCurve = nullptr;
inline GrannyCurveGetKnotCount_t GrannyCurveGetKnotCount = nullptr;
inline GrannyCurveGetDimension_t GrannyCurveGetDimension = nullptr;
inline GrannyCurveGetDegree_t GrannyCurveGetDegree = nullptr;
inline GrannyCurveIdentityPosition_t GrannyCurveIdentityPosition = nullptr;
inline GrannyCurveIdentityOrientation_t GrannyCurveIdentityOrientation = nullptr;
inline GrannyCurveIdentityScaleShear_t GrannyCurveIdentityScaleShear = nullptr;
inline GrannyCurveIdentityScale_t GrannyCurveIdentityScale = nullptr;
inline GrannyEvaluateCurveAtT_t GrannyEvaluateCurveAtT = nullptr;
inline GrannyEvaluateCurveAtKnotIndex_t GrannyEvaluateCurveAtKnotIndex = nullptr;
inline GrannyFindKnot_t GrannyFindKnot = nullptr;
inline GrannyFindCloseKnot_t GrannyFindCloseKnot = nullptr;
inline GrannyCurveIsKeyframed_t GrannyCurveIsKeyframed = nullptr;
inline GrannyCurveInitializeFormat_t GrannyCurveInitializeFormat = nullptr;
inline GrannyCurveDataDaIdentityType_t GrannyCurveDataDaIdentityType = nullptr;
inline GrannyTextureHasAlpha_t GrannyTextureHasAlpha = nullptr;
inline GrannyRGBA8888PixelFormat_t GrannyRGBA8888PixelFormat = nullptr;
inline GrannyRGB888PixelFormat_t GrannyRGB888PixelFormat = nullptr;
inline GrannyCopyTextureImage_t GrannyCopyTextureImage = nullptr;

///
/// \brief Returns function pointer to a function from granny2_x64.dll.
/// \param hModule Granny library
/// \param lpProcName Function name
/// \return
///
template <typename T>
T GetGrannyFunction(HMODULE hModule, const char* lpProcName);

///
/// \brief Initialize functions from granny2_x64.dll.
///
/// Assigns required type defined functions from granny2_x64.dll to declarations above
/// so that the declared functions above can be used during the import.
///
/// \return Success or failure
///
bool InitializeGrannyLibrary();
