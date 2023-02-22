using AutoMapper;
using ICSProject.DAL.Entities;

namespace ICSProject.BL.Models
{
    public record RideListModel(
        DateTime StarTime,
        DateTime EndTime,
        string Start,
        string End,
        uint Price) : ModelBase
    {
        public DateTime StarTime { get; set; } = StarTime;
        public DateTime EndTime { get; set; } = EndTime;
        public string Start { get; set; } = Start;
        public string End { get; set; } = End;
        public uint Price { get; set; } = Price;
        public string DriverName { get; set; }
        public string? DriverImageUrl { get; set; }
        public List<PassengerRideDetailModel> PassengerRides { get; init; } = new();
        public List<SemiStopDetailModel> SemiStops { get; init; } = new();
        public uint CarNumberOfSeats { get; set; }
        public int? SeatsLeft { get; set; }




        public class MapperProfile : Profile
        {
            public MapperProfile()
            {
                CreateMap<RideEntity, RideListModel>();
            }
        }
    }
}