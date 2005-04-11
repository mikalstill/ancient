using System.Collections;

namespace OpenPdf
{
	public class ObjectReferenceCollection : CollectionBase
	{
		public void Add(ObjectReference obj)
		{
			List.Add(obj);
		}
		
		public ObjectReference this[int i]
		{
			get
			{
				return (ObjectReference) List[i];
			}
		}
	}
}