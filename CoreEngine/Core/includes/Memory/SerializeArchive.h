#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>
#include <Math/includes/Transform.h>
#include <nlohmann/json.hpp>
#include <istream>

class FTransform;
class Object;

namespace CoreEngine
{
	class SerializeAchive
	{
	public:
		// Each object record has an absolute path, independent of its caller.
		class ObjectScope
		{
		public:
			ObjectScope(SerializeAchive& Archive, uint64 ID);
			~ObjectScope();
			ObjectScope(const ObjectScope&) = delete;
			ObjectScope& operator=(const ObjectScope&) = delete;
		private:
			SerializeAchive& Archive;
			DArray<String> PreviousPrefixes;
		};

		bool TrySerializeObject(const Object* Value);
		nlohmann::json MakeReference(const Object* Value) const;

		template<class T>
		void SerializeData(const String& NameData, const T& Data)
		{
			if (Prefixes.empty())
			{
				DataSave[NameData] = Data;
			}
			else
			{
				nlohmann::json& LastNode = FindLastNode();
				(LastNode)[NameData] = Data;
			}
		}

		template<class T>
		T DeserializeData(const String& NameData, bool& IsSuccess)
		{
			IsSuccess = true;
			if (Prefixes.empty())
			{
				if (!HasData(NameData))
				{
					IsSuccess = false;
					return T();
				}
				// = DataSave.find(NameData);
				return DataSave[NameData].get<T>();
			}
			else
			{
				nlohmann::json& LastNode = FindLastNode();
				if (!HasDataNode(LastNode, NameData))
				{
					IsSuccess = false;
					return T();
				}
				return LastNode[NameData].get<T>();
			}			
		}

		
		template<>
		FVector DeserializeData(const String& NameData, bool& IsSuccess)
		{

			IsSuccess = true;
			if (Prefixes.empty())
			{
				if (!HasData(NameData))
				{
					IsSuccess = false;
					return FVector();
				}
				auto& Vector = DataSave[NameData].get<DArray<float>>();
				return FVector(Vector[0], Vector[1], Vector[2]);
			}
			else
			{
				nlohmann::json& LastNode = FindLastNode();
				if (!HasDataNode(LastNode, NameData))
				{
					IsSuccess = false;
					return FVector();
				}
				auto& Vector = LastNode[NameData].get<DArray<float>>();

				return FVector(Vector[0], Vector[1], Vector[2]);
			}
		}
		template<>
		FTransform DeserializeData<FTransform>(const String& NameData, bool& IsSuccess)
		{
			IsSuccess = true;
			if (Prefixes.empty())
			{
				if (!HasData(NameData))
				{
					IsSuccess = false;
					return FTransform();
				}
				auto& LocationVector = DataSave[NameData]["Location"].get<DArray<float>>();
				auto& RotationVector = DataSave[NameData]["Rotation"].get<DArray<float>>();
				auto& ScaleVector = DataSave[NameData]["Scale"].get<DArray<float>>();
				return FTransform(FVector(LocationVector[0], LocationVector[1], LocationVector[2]), FVector(RotationVector[0], RotationVector[1], RotationVector[2]), FVector(ScaleVector[0], ScaleVector[1], ScaleVector[2]));
			}
			else
			{
				nlohmann::json& LastNode = FindLastNode();
				if (!HasDataNode(LastNode, NameData))
				{
					IsSuccess = false;
					return FTransform();
				}
				auto& LocationVector = LastNode[NameData]["Location"].get<DArray<float>>();
				auto& RotationVector = LastNode[NameData]["Rotation"].get<DArray<float>>();
				auto& ScaleVector = LastNode[NameData]["Scale"].get<DArray<float>>();

				return FTransform(FVector(LocationVector[0], LocationVector[1], LocationVector[2]), FVector(RotationVector[0], RotationVector[1], RotationVector[2]), FVector(ScaleVector[0], ScaleVector[1], ScaleVector[2]));
			}
		}


		bool HasData(const String& KeyName);

		void SerializeData(const String& NameData, const FVector& Data);
		void SerializeData(const String& NameData, const FTransform& Data);

		nlohmann::json& Data();
		void AddSerializedData(const SerializeAchive& NewNode);

		nlohmann::json& CreateNode();

		void ClearData();
		bool IsEmpty();

		void PushPrefix(const String& Data);
		void PopPrefix();
		nlohmann::json& FindLastNode();
		nlohmann::json& GetNodePrefix();


	public:


		friend std::istream& operator>>(std::istream& stream, SerializeAchive& j);

		SerializeAchive() = default;
		SerializeAchive(nlohmann::json& Data);
		SerializeAchive(const SerializeAchive&) = delete;
		SerializeAchive& operator=(const SerializeAchive&) = delete;


	private:

		nlohmann::json& FindDeserializeLastNode();
		bool HasDataNode(nlohmann::json& Node, const String& NameKay);

	private:

		DArray<String> Prefixes;
		HashTableMap<uint64, const Object*> SerializedObjects;
		nlohmann::json DataSave;// = YAML::Node(YAML::NodeType::Map);
		nlohmann::json MissingNode;
		bool IsDeserializing{false};


	};
}
