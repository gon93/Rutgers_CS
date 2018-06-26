using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Post
{
    class Program
    {
        static void Main(string[] args)
        {

            Console.WriteLine("Create a Post:");
            Console.WriteLine("1:Type Title");
            var title = Console.ReadLine();
            Console.WriteLine("2:Type Description");
            var description = Console.ReadLine();
            var post1 = new PostVote(DateTime.Now,title,description);
            post1.UpVote();
            post1.DownVote();
            post1.UpVote();
            post1.UpVote();
            post1.UpVote();
            post1.UpVote();
            post1.UpVote();

            Console.WriteLine("\nTitle: {0} \nDescription\n{1} \n{2}\nLikes: {3}",post1.Title,post1.Description,post1.Date,post1.Vote);
        }
    }
}
