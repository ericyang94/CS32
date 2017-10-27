// Homework 3 Problem 1

class Sport
{
	public:
		Sport(string nm)
		{
			m_name = nm;
		}
		virtual ~Sport()
		{
		}
		string name() const
		{
			return m_name;
		}
		virtual bool isOutdoor() const = 0;
		virtual string icon() const = 0;
	private:
		string m_name;

};

class Snowboarding: public Sport
{
	public:
		Snowboarding(string nm): Sport(nm)
		{
			m_outdoor = true;
			m_icon = "a descending snowboarder";
		}
		virtual ~Snowboarding()
		{
			cout << "Destroying the Snowboarding object named " << name() << "." << endl;
		}

		virtual bool isOutdoor() const
		{
			return m_outdoor;
		}
		virtual string icon() const
		{
			return m_icon;
		}
	private:
		bool m_outdoor;
		string m_icon;
};

class FigureSkating: public Sport
{
	public:
		FigureSkating(string nm): Sport(nm)
		{
			m_outdoor = false;
			m_icon = "a skater in the Biellmann position";
		}
		virtual ~FigureSkating()
		{
			cout << "Destroying the FigureSkating object named " << name() << "." << endl;
		}

		virtual bool isOutdoor() const
		{
			return m_outdoor;
		}
		virtual string icon() const
		{
			return m_icon;
		}
	private:
		bool m_outdoor;
		string m_icon;
};

class Biathlon: public Sport
{
	public:
		Biathlon(string nm, double dst): Sport(nm)
		{
			m_outdoor = true;
			m_icon = "a skier with a rifle";
			m_distance = dst;
		}
		virtual ~Biathlon()
		{
			cout << "Destroying the Biathlon object named " << name() << ", distance " << distance() <<" km." << endl;
		}
		virtual bool isOutdoor() const
		{
			return m_outdoor;
		}
		virtual string icon() const
		{
			return m_icon;
		}
		double distance() const
		{
			return m_distance;
		}
	private:
		bool m_outdoor;
		string m_icon;
		double m_distance;
};