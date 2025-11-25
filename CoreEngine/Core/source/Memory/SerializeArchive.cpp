#include <Core/includes/Memory/SerializeArchive.h>

#include <nlohmann/json.hpp>

#include <Math/includes/Transform.h>

namespace CoreEngine
{
	//YAML::Node SerializeAchive::DataSave = YAML::Node(YAML::NodeType::Map);

	void SerializeAchive::SerializeData(const String& NameData, const FVector& Data)
	{
		std::stringstream stream;
		stream << "[" << Data.GetX() << "," << Data.GetY() << "," << Data.GetZ() << "]";
		if (Prefixes.empty())
		{
			DataSave[NameData] = (stream.str());
		}
		else
		{
			nlohmann::json& LastNode = FindLastNode();
			LastNode[NameData] = (stream.str());
		}
	}
	
	void SerializeAchive::SerializeData(const String& NameData, const FTransform& Data)
	{
		std::stringstream streamLocation;
		streamLocation << "[" << Data.GetLocation().GetX() << "," << Data.GetLocation().GetY() << "," << Data.GetLocation().GetZ() << "]";

		
		std::stringstream streamRotation;
		streamRotation << "[" << Data.GetRotation().GetX() << "," << Data.GetRotation().GetY() << "," << Data.GetRotation().GetZ() << "]";
		
		std::stringstream streamScale;
		streamScale << "[" << Data.GetScale().GetX() << "," << Data.GetScale().GetY() << "," << Data.GetScale().GetZ() << "]";
		if (Prefixes.empty())
		{
			DataSave[NameData]["Location"] = (streamLocation.str());
			DataSave[NameData]["Rotation"] = (streamRotation.str());
			DataSave[NameData]["Scale"] = (streamScale.str());
		}
		else
		{
		
			nlohmann::json& LastNode = FindLastNode();

			LastNode[NameData]["Location"] = (streamLocation.str());
			LastNode[NameData]["Rotation"] = (streamRotation.str());
			LastNode[NameData]["Scale"] = (streamScale.str());
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
			//if (!LastNode.) return nlohmann::json();
			
			//nlohmann::json child = GetChildNoCreate(LastNode, Prefix);
	
			auto child = LastNode->find(Prefix);
			
			if (!LastNode->count(Prefix))
			{
				
				(*LastNode)[Prefix] = nlohmann::json({});
				child = LastNode->find(Prefix);
				//child = GetChildNoCreate(LastNode, Prefix);
			}
			
			LastNode = &(*child);
			
			
		}
	
		return *LastNode;
	}
}