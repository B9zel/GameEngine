#include <Core/includes/Memory/SerializeArchive.h>

#include <nlohmann/json.hpp>

#include <Math/includes/Transform.h>

namespace CoreEngine
{
	//YAML::Node SerializeAchive::DataSave = YAML::Node(YAML::NodeType::Map);

	bool SerializeAchive::HasData(const String& KeyName)
	{
		return HasDataNode(DataSave, KeyName);
	}

	void SerializeAchive::SerializeData(const String& NameData, const FVector& Data)
	{
		static DArray<float> VectorSerialized;
		VectorSerialized.clear();
		VectorSerialized = { Data.GetX(), Data.GetY(), Data.GetZ() };

		//stream << "[" << Data.GetX() << "," << Data.GetY() << "," << Data.GetZ() << "]";
		if (Prefixes.empty())
		{
			DataSave[NameData] = VectorSerialized;
		}
		else
		{
			nlohmann::json& LastNode = FindLastNode();
			LastNode[NameData] = VectorSerialized;
		}
	}
	
	void SerializeAchive::SerializeData(const String& NameData, const FTransform& Data)
	{
		static DArray<float> VectorSerializedLocation;
		VectorSerializedLocation.clear();
		VectorSerializedLocation = { Data.GetLocation().GetX(), Data.GetLocation().GetY(), Data.GetLocation().GetZ() };
		
		static DArray<float> VectorSerializedRotation;
		VectorSerializedRotation.clear();
		VectorSerializedRotation = { Data.GetRotation().GetX(), Data.GetRotation().GetY(), Data.GetRotation().GetZ() };
		
		static DArray<float> VectorSerializedScale;
		VectorSerializedScale.clear();
		VectorSerializedScale = { Data.GetScale().GetX(), Data.GetScale().GetY(), Data.GetScale().GetZ() };
	
		if (Prefixes.empty())
		{
			DataSave[NameData]["Location"] = VectorSerializedLocation;
			DataSave[NameData]["Rotation"] = VectorSerializedRotation;
			DataSave[NameData]["Scale"] = VectorSerializedScale;
		}
		else
		{
		
			nlohmann::json& LastNode = FindLastNode();

			LastNode[NameData]["Location"] = VectorSerializedLocation;
			LastNode[NameData]["Rotation"] = VectorSerializedRotation;
			LastNode[NameData]["Scale"] = VectorSerializedScale;
		}
	}

	nlohmann::json& SerializeAchive::Data()
	{
		return DataSave;
	}

	void SerializeAchive::AddSerializedData(const SerializeAchive& NewNode)
	{
		DataSave.push_back(NewNode.DataSave);
	}

	nlohmann::json& SerializeAchive::CreateNode()
	{
		return nlohmann::json();
	}

	void SerializeAchive::ClearData()
	{
		DataSave.clear();
	}
	bool SerializeAchive::IsEmpty()
	{
		return DataSave.empty();
	}

	void SerializeAchive::PushPrefix(const String& Data)
	{
		
		Prefixes.push_back(Data);
	}

	void SerializeAchive::PopPrefix()
	{
		
		Prefixes.pop_back();
	}

	void dump(const nlohmann::json& n, const std::string& name) {
		std::cout << "---- " << name << " ----\n";
		std::cout << "defined=" 
			<< " size=" << n.size() << "\n";
		std::cout << name << " YAML:\n" << n.dump() << "\n\n";
	}

	nlohmann::json& SerializeAchive::FindLastNode()
	{

		nlohmann::json* LastNode = &DataSave;
		for (auto& Prefix : Prefixes)
		{
			auto child = LastNode->find(Prefix);
			if (!LastNode->count(Prefix))
			{
				(*LastNode)[Prefix] = nlohmann::json({});
				child = LastNode->find(Prefix);
			}
			
			LastNode = &(*child);
		}
	
		return *LastNode;
	}
	nlohmann::json& SerializeAchive::FindDeserializeLastNode()
	{
		nlohmann::json* LastNode = &DataSave;
		for (auto& Prefix : Prefixes)
		{
			auto child = LastNode->find(Prefix);
			LastNode = &(*child);


		}

		return *LastNode;
	}
	bool SerializeAchive::HasDataNode(nlohmann::json& Node, const String& NameKay)
	{
		return Node.count(NameKay);
	}
	std::istream& operator>>(std::istream& stream, SerializeAchive& j)
	{
		stream >> j.DataSave;
		return stream;
	}
}