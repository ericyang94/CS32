void listAll(const Class* c, string path)  // two-parameter overload
{
	cout<<path+c->name()<<endl;
	if (c->subclasses().empty())
	{
		return;
	}
    string list = path+c->name()+"=>";
	for(int k = 0; k != c->subclasses().size(); k++)
		listAll(c->subclasses()[k], list);
}