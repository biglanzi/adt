
void debug_pb2json(const ::google::protobuf::Message& msg, rapidjson::Value& jv, rapidjson::Document &doc)
{
	auto desc = msg.GetDescriptor();
	auto refl = msg.GetReflection();

	for (auto i = 0; i < desc->field_count(); i++)
	{
		auto f = desc->field(i);
		auto const &fname = f->name();
		if (f->is_repeated())
		{

		}
		else
		{
			switch (f->cpp_type())
			{
			case  ::google::protobuf::FieldDescriptor::CPPTYPE_INT32:
				{
					auto value = refl->GetInt32(msg, f);

					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, doc.GetAllocator());
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_INT64:
				{
					auto value = refl->GetInt64(msg, f);

					jv.AddMember(rapidjson::StringRef(fname.c_str()), value, doc.GetAllocator());
				}
				break;
			case ::google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
				{
					rapidjson::Value temp(rapidjson::kObjectType);
					debug_printf(refl->GetMessage(msg, f), temp, doc);
					jv.AddMember(rapidjson::StringRef(fname.c_str()), temp, doc.GetAllocator());
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
	debug_pb2json(msg, jv, doc);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jv.Accept(writer);
	std::cout << string(buffer.GetString()) << std::endl;
}
