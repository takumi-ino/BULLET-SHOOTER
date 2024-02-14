#pragma once
#include "../../library/tnl_vector3.h"
#include "../../library/tnl_matrix.h"
#include "../../library/tnl_quaternion.h"

#include <string>
#include "../../library/tnl_using.h"
#include "../dxlib_ext_texture.h"
#include "../dxlib_ext_data.h"
#include "dxlib_ext_mesh_base.h"

#undef LONG_PTR
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

namespace dxe {
	class Mesh;
	class InstMesh;

	//// <summary>
	/// �ϐ��̖����K��
	/// �e���̒e�Ƃ��Ďg�p����ꍇ
	/// 1. _bltMeshPool   (Bullet Mesh Pool �̗��j
	/// 2. _spell         (�e���̖��́j
    /// 3. _name          (�g�p�L�����N�^�[�̖��O�j
	///  ����@_bltMeshPool_silentSerena_patchouli
	//// </summary>
	class InstMeshPool final : public MeshBase {
	public:
		~InstMeshPool() {}

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// ���b�V���v�[���̍쐬
		// arg1... ���₵�������b�V��
		// arg2... ���₷��
		// arg3... �g�p����e�N�X�`���z��
		static Shared<InstMeshPool> Create(const Shared<Mesh>& mesh, uint32_t instance_num, const std::vector<Shared<dxe::Texture>>* textures = nullptr);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �ʂ̃C���X�^���V���O���b�V���̎擾
		// tips... ����ꂽ���b�V������������Ǝ����I�Ƀv�[���ɕԊ҂���܂�
		Shared<InstMesh> CreateInstMesh();

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �C���X�^���X�̍ő吔
		uint32_t getMaxInstanceNum() { return instance_num_; }

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �擾�\�Ȏc��̃C���X�^���X��
		uint32_t getFreeInstanceNum() { return (uint32_t)free_instance_stack_.size(); }

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �`��
		void render(Shared<dxe::Camera> camera);

		//-----------------------------------------------------------------------------------------------------------------------------------------------------
		// �Օ����Ƃ��ăV���h�E�}�b�v�̏���
		// tips... �Օ����Ƃ��ċ@�\���܂����A�����ŃC���X�^���X�̐��������`�悪���s����邽�ߑ����̃R�X�g�ɂȂ�܂�
		void reserveShadow();


	private:
		friend class InstMesh;

		class Instance final {
		public:
			~Instance();
			const tnl::Vector3& getPosition() { return pos_; }
			const tnl::Vector3& getScale() { return scl_; }
			const tnl::Quaternion& getRotation() { return rot_; }
			uint32_t getUseTextureIndex() { return use_tex_index_; }
			bool getDrawEnable() { return draw_enable_; }

			void setPosition(const tnl::Vector3& pos);
			void setScale(const tnl::Vector3& scl);
			void setRotation(const tnl::Quaternion& rot);
			void setUseTextureIndex(uint32_t index);
			void setDrawEnable(bool enable);
		private:
			friend class InstMesh;
			friend class InstMeshPool;
			Instance(const Shared<InstMeshPool>& pool);
			bool draw_enable_ = false;
			uint32_t use_tex_index_ = 0;
			tnl::Vector3 pos_;
			tnl::Vector3 scl_ = { 1,1,1 };
			tnl::Quaternion rot_;
			Weak<InstMeshPool> ref_mesh_pool_;
		};

		InstMeshPool(const Shared<Mesh>& mesh, uint32_t instance_num, const std::vector<Shared<dxe::Texture>>* textures = nullptr);
		void mapInstances();


		TNL_PROPERTY(bool, NeedsUpdated, is_needs_updated_);
		bool						is_needs_updated_ = true;
		uint32_t					ref_texture_index_num_ = 0;
		uint32_t					instance_num_ = 0;
		uint32_t					index_num_ = 0;
		eBlendState					blend_state_ = eBlendState::NORMAL;
		Shared<Mesh>				origine_clone_;
		std::vector<Shared<Instance>>	instances_;
		std::stack<Shared<Instance>>	free_instance_stack_;

		// ���_��`
		typedef struct _VERTEX
		{
			DirectX::XMFLOAT3 position_;
			DirectX::XMFLOAT3 normal_;
			DirectX::XMFLOAT2 tex_coord_;
		} INST_MESH_VERTEX;

		// ���_�o�b�t�@
		ComPtr<ID3D11Buffer>				vertex_buffer_ = nullptr;
		// �C���X�^���X�o�b�t�@
		ComPtr<ID3D11Buffer>				instance_buffer_ = nullptr;
		// �C���f�b�N�X�o�b�t�@
		ComPtr<ID3D11Buffer>				index_buffer_ = nullptr;
		// ���_�V�F�[�_�[
		ComPtr<ID3D11VertexShader>          vertex_shader_ = nullptr;
		// �s�N�Z���V�F�[�_�[
		ComPtr<ID3D11PixelShader>           pixel_shader_ = nullptr;

		// ���_���̓��C�A�E�g
		ComPtr<ID3D11InputLayout>           input_layout_ = nullptr;
		// �萔�o�b�t�@
		ComPtr<ID3D11Buffer>                constant_buffer_ = nullptr;

		// �V�F�[�_�[���\�[�X�r���[
		ComPtr<ID3D11Texture2D>			    texture_resouces_;
		ComPtr<ID3D11ShaderResourceView>	shader_resouce_views_;

		// ���X�^���C�U�X�e�[�g
		ComPtr<ID3D11RasterizerState>		rasterizer_state_ = nullptr;
		// �T���v���[�X�e�[�g
		ComPtr<ID3D11SamplerState>          sampler_state_ = nullptr;

	};


	class InstMesh final {
	public:

		~InstMesh();
		const tnl::Vector3& getPosition();
		const tnl::Vector3& getScale();
		const tnl::Quaternion& getRotation();
		uint32_t getUseTextureIndex();
		bool getDrawEnable();

		void setPosition(const tnl::Vector3& pos);
		void setScale(const tnl::Vector3& scl);
		void setRotation(const tnl::Quaternion& rot);
		void setUseTextureIndex(uint32_t index);
		void setDrawEnable(bool enable);

	private:
		friend class InstMeshPool;
		InstMesh() = default;
		InstMesh(Shared<InstMeshPool::Instance> instance) : ref_instance_(instance) {}
		Shared<InstMeshPool::Instance> ref_instance_ = nullptr;
	};


}