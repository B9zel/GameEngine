#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>
#include <nlohmann/json.hpp>

class FTransform;

namespace CoreEngine
{
	class SerializeAchive
	{
	public:

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


		void SerializeData(const String& NameData, const FVector& Data);
		void SerializeData(const String& NameData, const FTransform& Data);

		nlohmann::json& Data();
		void AddSerializedData(const SerializeAchive& NewNode);

		nlohmann::json& CreateNode();

		void ClearData();
		bool IsEmpty();

		void PushPrefix(const String& Data);
		void PopPrefix();

	private:

		nlohmann::json& FindLastNode();

	private:

		DArray<String> Prefixes;
		nlohmann::json DataSave;// = YAML::Node(YAML::NodeType::Map);


	};
}