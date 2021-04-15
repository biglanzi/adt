#include <iostream>
#include <string>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

void debug_pb2json(const ::google::protobuf::Message& msg, rapidjson::Value& jv, rapidjson::Document &doc)
{
	auto desc = msg.GetDescriptor();
	auto reflect = msg.GetReflection();
	auto &alloc = doc.GetAllocator();
	for (auto i = 0; i < desc->field_count(); i++)
	{
		auto f = desc->field(i);
		auto const &fname = f->name();
		if (f->is_repeated())
		{
			if (reflect->FieldSize(msg, f) == 0)
			{
				continue;
			}
			rapidjson::Value arr(rapidjson::kArrayType);
			for (auto idx = 0; idx < reflect->FieldSize(msg, f); ++idx)
			{
				switch (f->cpp_type())
				{
				case  ::google::protobuf::FieldDescriptor::CPPTYPE_INT32:
					{
						auto value = reflect->GetRepeatedInt32(msg, f, idx);
						arr.PushBack(value, alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_INT64:
					{
						auto value = reflect->GetRepeatedInt64(msg, f, idx);
						arr.PushBack(value, alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
					{
						auto value = reflect->GetRepeatedUInt32(msg, f, idx);
						arr.PushBack(value, alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
					{
						auto value = reflect->GetRepeatedUInt64(msg, f, idx);
						arr.PushBack(value, alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
					{
						auto value = reflect->GetRepeatedDouble(msg, f, idx);
						arr.PushBack(value, alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
					{
						auto value = reflect->GetRepeatedFloat(msg, f, idx);
						arr.PushBack(value, alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
					{
						auto value = reflect->GetRepeatedBool(msg, f, idx);
						arr.PushBack(value, alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_STRING:
					{
						auto value = std::move(reflect->GetRepeatedString(msg, f, idx));
						arr.PushBack(rapidjson::Value(value.c_str(), value.length()), alloc);
					}
					break;
				case ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
					{
						rapidjson::Value temp(rapidjson::kObjectType);
						debug_pb2json(reflect->GetRepeatedMessage(msg, f, idx), temp, doc);
						arr.PushBack(temp, alloc);
					}
					break;
				default:
					break;
				}
			}
			jv.AddMember(rapidjson::StringRef(fname.c_str()), arr, alloc);
		}
		else
		{
			if (!reflect->HasField(msg, f))
			{
				continue;
			}
			switch (f->cpp_type())
			{
			case  ::google::protobuf::FieldDescriptor::CPPTYPE_INT32:
				{
					auto value = reflect->GetInt32(msg, f);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_INT64:
				{
					auto value = reflect->GetInt64(msg, f);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
				{
					auto value = reflect->GetUInt32(msg, f);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
				{
					auto value = reflect->GetUInt64(msg, f);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
				{
					auto value = reflect->GetDouble(msg, f);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
				{
					auto value = reflect->GetFloat(msg, f);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
				{
					auto value = reflect->GetBool(msg, f);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_STRING:
				{
					auto value = std::move(reflect->GetString(msg, f));
					jv.AddMember(rapidjson::StringRef(fname.c_str()), rapidjson::Value(value.c_str(), value.length()), alloc);
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
				{
					rapidjson::Value temp(rapidjson::kObjectType);
					debug_pb2json(reflect->GetMessage(msg, f), temp, doc);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), temp, alloc);
				}
				break;
			default:
				break;
			}
		}
	}
}

int main()
{
	rapidjson::Document doc;
	rapidjson::Value jv(rapidjson::kObjectType);
	
	debug_pb2json(pbtest, jv, doc);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jv.Accept(writer);
	std::cout << std::string(buffer.GetString()) << std::endl;
	getchar();
}
