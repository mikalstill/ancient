using System.Collections;

namespace OpenPdf
{
	public class ObjectCollection : CollectionBase
	{
		public void Add(Object obj)
		{
			List.Add(obj);
		}
		
		public Object Get(ObjectReference or)
		{
			return Get(or.Number, or.Generation);
		}
		
		public Object Get(int number, int generation)
		{
			foreach(Object o in List)
			{
				if((o.Number == number) && (o.Generation == generation))
				{
					return o;
				}
			}
			
			return new Object();
		}
		
		public ObjectCollection ByType(string type)
		{
			ObjectCollection oc = new ObjectCollection();
			foreach(Object o in List)
			{
				if((o.Valid) && (o.Type == type))
					oc.Add(o);
			}
			
			return oc;
		}
	}
}