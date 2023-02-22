using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Windows;
using System;
using System.Linq;
using ICSProject.BL.Models;
using System.Collections.ObjectModel;
using ICSProject.App.Extensions;

namespace ICSProject.App.Wrappers
{
    public class UserWrapper : ModelWrapper<UserDetailModel>
    {
        public UserWrapper(UserDetailModel model) : base(model) 
        {
            //InitializeCollectionProperties(model);
        }
        public string? Name
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public string? Email
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public string? TelephoneNumber
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public DateTime? Birthday
        {
            get => GetValue<DateTime>();
            set => SetValue(value);
        }
        public string? Hometown
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public string? ImageUrl
        {
            get => GetValue<string>();
            set => SetValue(value);
        }

        private void InitializeCollectionProperties(UserDetailModel model)
        {
            /*if (model.Cars == null)
            {
                throw new ArgumentException("Cars cannot be null");
            }
            Cars.AddRange(model.Cars.Select(e => new CarWrapper(e)));

            RegisterCollection(Cars, model.Cars);

          */
        }

        public ObservableCollection<CarWrapper> Cars { get; set; } = new();
        public ObservableCollection<RideWrapper> Rides { get; set; } = new();

        public override IEnumerable<ValidationResult> Validate(ValidationContext validationContext)
        {
            if (string.IsNullOrWhiteSpace(Name))
            {
                yield return new ValidationResult($"{nameof(Name)} is required", new[] { nameof(Name) });
            }
        }

        public static implicit operator UserWrapper(UserDetailModel detailModel)
            => new(detailModel);

        public static implicit operator UserDetailModel(UserWrapper wrapper)
            => wrapper.Model;
    }
}