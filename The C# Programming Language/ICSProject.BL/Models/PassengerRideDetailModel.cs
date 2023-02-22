using AutoMapper;
using ICSProject.DAL.Entities;

namespace ICSProject.BL.Models
{
    public record PassengerRideDetailModel(
        Guid PassengerId,
        Guid RideId,
        string PassengerName,
        string? PassengerImageUrl) : ModelBase
    {
        public Guid PassengerId { get; set; } = PassengerId;
        public Guid RideId { get; set; } = RideId;
        public string PassengerName { get; set; } = PassengerName;
        public string PassengerImageUrl { get; set; } = PassengerImageUrl;

        public class MapperProfile : Profile
        {
            public MapperProfile()
            {
                CreateMap<PassengerRideEntity, PassengerRideDetailModel>()
                    .ReverseMap()
                    .ForMember(entity => entity.Passenger, expression => expression.Ignore());
            }
        }

        public static PassengerRideDetailModel Empty => new (default, default, default, string.Empty);
    }
}