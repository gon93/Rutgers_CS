using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Principal;
using System.Text;

namespace Post
{
    class PostVote
    {
        private int vote = 0;
        public string Title { get; private set; }
        public string Description { get; private set; }
        public DateTime Date { get; private set; }

        public PostVote(DateTime date, string title, string description)
        {
            Date = date;
            Title = title;
            Description = description;
        }

        

        public int UpVote()
        {
            vote++;
            return vote;
        }
        public int DownVote()
        {
            vote--;
            return vote;
        }
        public int Vote
        {
            get { return vote;}
        }
    }
}
