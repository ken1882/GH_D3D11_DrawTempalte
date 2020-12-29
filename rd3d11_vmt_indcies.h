#ifndef __RD3D11_VMT__INDCIES_H__
#define __RD3D11_VMT__INDCIES_H__


enum class IDXGISwapChainVMT {
	QueryInterface,
	AddRef,
	Release,
	SetPrivateData,
	SetPrivateDataInterface,
	GetPrivateData,
	GetParent,
	GetDevice,
	Present,
	GetBuffer,
	SetFullscreenState,
	GetFullscreenState,
	GetDesc,
	ResizeBuffers,
	ResizeTarget,
	GetContainingOutput,
	GetFrameStatistics,
	GetLastPresentCount,
};

enum class ID3D11DeviceVMT {
	QueryInterface,
	AddRef,
	Release,
	CreateVideoDecoder,
	CreateVideoProcessor,
	CreateAuthenticatedChannel,
	CreateCryptoSession,
	CreateVideoDecoderOutputView,
	CreateVideoProcessorInputView,
	CreateVideoProcessorOutputView,
	CreateVideoProcessorEnumerator,
	GetVideoDecoderProfileCount,
	GetVideoDecoderProfile,
	CheckVideoDecoderFormat,
	GetVideoDecoderConfigCount,
	GetVideoDecoderConfig,
	GetContentProtectionCaps,
	CheckCryptoKeyExchange,
	SetPrivateData,
	SetPrivateDataInterface,
};

enum class ID3D11DeviceContextVMT {
	QueryInterface,
	AddRef,
	Release,
	GetDevice,
	GetPrivateData,
	SetPrivateData,
	SetPrivateDataInterface,
	VSSetConstantBuffers,
	PSSetShaderResources,
	PSSetShader,
	PSSetSamplers,
	VSSetShader,
	DrawIndexed,
	Draw,
	Map,
	Unmap,
	PSSetConstantBuffers,
	IASetInputLayout,
	IASetVertexBuffers,
	IASetIndexBuffer,
	DrawIndexedInstanced,
	DrawInstanced,
	GSSetConstantBuffers,
	GSSetShader,
	IASetPrimitiveTopology,
	VSSetShaderResources,
	VSSetSamplers,
	Begin,
	End,
	GetData,
	SetPredication,
	GSSetShaderResources,
	GSSetSamplers,
	OMSetRenderTargets,
	OMSetRenderTargetsAndUnorderedAccessViews,
	OMSetBlendState,
	OMSetDepthStencilState,
	SOSetTargets,
	DrawAuto,
	DrawIndexedInstancedIndirect,
	DrawInstancedIndirect,
	Dispatch,
	DispatchIndirect,
	RSSetState,
	RSSetViewports,
	RSSetScissorRects,
	CopySubresourceRegion,
	CopyResource,
	UpdateSubresource,
	CopyStructureCount,
	ClearRenderTargetView,
	ClearUnorderedAccessViewUint,
	ClearUnorderedAccessViewFloat,
	ClearDepthStencilView,
	GenerateMips,
	SetResourceMinLOD,
	GetResourceMinLOD,
	ResolveSubresource,
	ExecuteCommandList,
	HSSetShaderResources,
	HSSetShader,
	HSSetSamplers,
	HSSetConstantBuffers,
	DSSetShaderResources,
	DSSetShader,
	DSSetSamplers,
	DSSetConstantBuffers,
	CSSetShaderResources,
	CSSetUnorderedAccessViews,
	CSSetShader,
	CSSetSamplers,
	CSSetConstantBuffers,
	VSGetConstantBuffers,
	PSGetShaderResources,
	PSGetShader,
	PSGetSamplers,
	VSGetShader,
	PSGetConstantBuffers,
	IAGetInputLayout,
	IAGetVertexBuffers,
	IAGetIndexBuffer,
	GSGetConstantBuffers,
	GSGetShader,
	IAGetPrimitiveTopology,
	VSGetShaderResources,
	VSGetSamplers,
	GetPredication,
	GSGetShaderResources,
	GSGetSamplers,
	OMGetRenderTargets,
	OMGetRenderTargetsAndUnorderedAccessViews,
	OMGetBlendState,
	OMGetDepthStencilState,
	SOGetTargets,
	RSGetState,
	RSGetViewports,
	RSGetScissorRects,
	HSGetShaderResources,
	HSGetShader,
	HSGetSamplers,
	HSGetConstantBuffers,
	DSGetShaderResources,
	DSGetShader,
	DSGetSamplers,
	DSGetConstantBuffers,
	CSGetShaderResources,
	CSGetUnorderedAccessViews,
	CSGetShader,
	CSGetSamplers,
	CSGetConstantBuffers,
	ClearState,
	Flush,
	GetType,
	GetContextFlags,
	FinishCommandList,
};

#endif // __RD3D11_VMT__INDCIES_H__
