namespace ICSProject.DAL.Entities
{
    public record UserEntity(
        Guid Id,
        string Name,
        string Email,
        string TelephoneNumber,
        DateTime Birthday,
        string Hometown,
        string? ImageUrl) : IEntity
    {

#nullable disable
        public UserEntity() : this(default, default, default, default, default, default, default) { }
#nullable enable
        public ICollection<CarEntity> Cars { get; init; } = new List<CarEntity>();
        public ICollection<RideEntity> DriverRides { get; init; } = new List<RideEntity>();
        public ICollection<PassengerRideEntity> PassengerRides { get; init; } = new List<PassengerRideEntity>();
    }
}

