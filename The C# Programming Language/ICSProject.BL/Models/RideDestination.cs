using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace ICSProject.BL.Models
{
    public record RideDestination(string destination, string time)
    {
        public string Destination { get; set; } = destination;
        public string Time { get; set; } = time;
    }
}
