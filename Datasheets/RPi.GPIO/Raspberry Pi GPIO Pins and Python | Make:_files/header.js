(function ($) { $(function() {
  'use strict';
	var isiPhone = navigator.userAgent.toLowerCase().indexOf("iphone");
	var searchFocus = false;
	var lessThan768;
	var viewWidth;
	if(window.navigator.userAgent.indexOf("Chrome") > 0){
		viewWidth = $(window).width() + 17;
	}else {
		viewWidth = $(window).width() ;
	}
	if (viewWidth >= '768') {
		lessThan768 = false;
	} else {
		lessThan768 = true;
	}
	$(window).scroll(function () {
		$(window).resize(function () {
			viewWidth = $(window).width() + 17;
		});
		var scrollTop = $(window).scrollTop();
		if (viewWidth >= '481') {
			$(window).resize(function () {
				viewWidth = $(window).width() + 17;
			});
			// menu-item-has-children = navbar items with dropdowns
			// dynamic-header-posts = dropdown boxes in the navbar
			if (scrollTop >= '119') {
				$('.menu-item-has-children').children("a").removeClass("active-button");
				if ($('.dynamic-header-posts').hasClass('sticky-header')) {
					$('.dynamic-header-posts').slideUp('fast');
				} else {
					$('.dynamic-header-posts').hide();
				}
				window.setTimeout(function () {
					$('.latest-projects').hide().removeClass('nav-transition');
					$('.latest-stories').hide().removeClass('nav-transition');
					$('.latest-events').hide().removeClass('nav-transition');
					$('.latest-shop').hide().removeClass('nav-transition');
					$('.nav-guides').hide().removeClass('nav-transition');
					$('.nav-share').hide().removeClass('nav-transition');
					$('.dynamic-header-content').children().removeClass('moove-left moove-right');
					$('.menu-sub-menu > .sub-menu').hide().removeClass('nav-transition');
					$('.menu-sub-menu > .sub-menu').removeClass('moove-left moove-right');
				}, 0);
				$('.menu-item-has-children').addClass('first-hover');
				$('.container.header').addClass('width');
				$('.navbar-default').addClass('sticky-header');
				$('.dynamic-header-posts').addClass('sticky-header');
				if ($('.header').hasClass('tablet')) {
					$('input.search-field').bind('focusin focus', function () {
						window.scrollTo(0, 0);
						document.body.scrollTop = 0;
						searchFocus = true;
					});
					$('input.search-field').focusout(function () {
						$(".container.header.width").css("position", "");
						$(".container.header.width").css("margin-top", "");
						searchFocus = false;
					});
					if (searchFocus === true) {
						$(".container.header.width").css("margin-top", scrollTop);
						$(".container.header.width").css("position", 'absolute');
					}
				}
				else {
					$(window).resize(function () {
						viewWidth = $(window).width() + 17;
					});
				}
				$('.menu-item-has-children a').addClass('sticky-a');
			} else {
				$('.container.header').removeClass('width');
				$('.navbar-default').removeClass('sticky-header');
				$('.menu-item-has-children a').removeClass('sticky-a');
				$('.dynamic-header-posts').removeClass('sticky-header').css('top', 'inherit');
				$('.navbar-default').css('top', '0px');
				$('.menu-item-has-children > a').removeClass("active-sticky");
				$('.second-nav').removeClass("displayNav");
				if ($('.header').hasClass('tablet')) {
					$(".container.header").css("position", "");
					$(".container.header").css("margin-top", "");
					$(".container.header").css("top", "");
				}
			}
			if (scrollTop >= '119') {
				$('.header-wrapper').addClass('mobile-sticky');
			} else {
				$('.header-wrapper').removeClass('mobile-sticky');
			}
		}

		if (viewWidth <= '767') {
			if (scrollTop >= '55') {
				$('.header-wrapper').addClass('mobile-sticky');
			} else {
				$('.header-wrapper').removeClass('mobile-sticky');
			}
		}
	});

	if(window.navigator.userAgent.indexOf("Chrome") > 0){
		viewWidth = $(window).width() + 17;
	}else {
		viewWidth = $(window).width() ;
	}
	$(window).resize(function () {
		if(window.navigator.userAgent.indexOf("Chrome") > 0){
			viewWidth = $(window).width() + 17;
		}else {
			viewWidth = $(window).width() ;
		}
		$('body').css('overflow-y', '').css('height', '');
		if (viewWidth <= '767') {
			$('.menu-item-has-children').addClass('first-hover');
			if (lessThan768 === false) {
				$('div.navbar-collapse').hide();
				lessThan768 = true;
			}
			$('.dynamic-header-posts').hide();
		} 
		else {
			if (lessThan768 === true) {
				$('div.navbar-collapse').show();
				$('.dynamic-header-posts').hide();
				$('li a').removeClass('active-button');
				$(".menu-item-has-children .sub-menu").hide();
				lessThan768 = false;
			}
			$('.get-dark').removeClass('show');
			$('.navbar-default .navbar-toggle').removeClass('close-background');
			$('.navbar-default .navbar-toggle .icon-bar').removeClass('hide-background');
		}
	});

	// Title Attribute
	$('#menu-make-main > li').each(function () {
		var menuContent;
		if ($(this).hasClass('menu-item-has-children')) {
			var title = $(this).children('a').attr('title');
			$(this).addClass(title);
			menuContent = $(this).find('.sub-menu');
			if (title == "Projects") {
				$(menuContent).addClass('projects');
			}
			if (title == "Stories") {
				$(menuContent).addClass('stories');
			}
			$(this).children('a').attr('title', '');
			$(menuContent).clone().appendTo('.menu-sub-menu');
		} else {
			$('.menu-sub-menu').append('<ul class="sub-menu"></ul>');
		}
	});

	$('.menu-item-has-children').children("a").click(function (e) {
		if(window.navigator.userAgent.indexOf("Chrome") > 0){
			viewWidth = $(window).width() + 17;
		}else {
			viewWidth = $(window).width() ;
		}
		if (viewWidth <= '767') {
			e.preventDefault();
			$(".menu-item-has-children").not($($(this).parent().get(0))).find('.sub-menu').slideUp('slow');
			$($(this).parent().get(0)).find('.sub-menu').slideToggle('slow');
			$(".menu-item-has-children").not($($(this).parent().get(0))).children("a").removeClass("active-button");
			$(this).toggleClass("active-button");
		}
	});

	// Mobile
	$('.navbar-toggle').on('click', function () {
		$('.navbar-default .navbar-toggle').toggleClass('close-background');
		$('.navbar-default .navbar-toggle .icon-bar').toggleClass('hide-background');
		if (!$('.get-dark').hasClass('show')) {
			window.setTimeout(function () {
				$('.menu-container').addClass('scroll');
			}, 600);
			$('body').css('overflow-y', 'hidden').css('height', '100vh');
			$('.navbar-default .navbar-collapse').show();
			$('.get-dark').addClass('show');
			$('.get-dark').animate({
				opacity: 0.5
			}, 250, function () {
			});
			$('.menu-container').addClass('menu-show');
			$('.sumome-share-client-wrapper.sumome-share-client-wrapper-mobile-bottom-bar').css('display', 'none');
		} else {
			$('body').css('overflow-y', '').css('height', '');
			$('.menu-container').removeClass('menu-show');
			$('.get-dark').removeClass('show');
		}
	});

	// Mobile
	$('.get-dark').on('click', function () {
		$('body').css('overflow-y', '').css('height', '');
		$('.get-dark').animate({
			opacity: 0
		}, 250, function () {
			$(this).removeClass('show');
		});
		$('.menu-container').removeClass('scroll menu-show');
		$('.navbar-default .navbar-toggle').removeClass('close-background');
		$('.navbar-default .navbar-toggle .icon-bar').removeClass('hide-background');
		$('.sumome-share-client-wrapper.sumome-share-client-wrapper-mobile-bottom-bar').css('display', '');
		if ($('.get-dark').hasClass('show')) {
			$('#makezine-navbar-collapse-1').removeClass('in');
		}
	});

	$('.search-field').attr('title', '');
	$('.menu-item-has-children').addClass('first-hover');
	$(document).on('mouseenter', '.menu-item-has-children', function (e) {
		if(window.navigator.userAgent.indexOf("Chrome") > 0){
			viewWidth = $(window).width() + 17;
		}else {
			viewWidth = $(window).width() ;
		}
		if (viewWidth >= '768') {
			e.preventDefault();
			$('.menu-item-has-children').not($(this)).children("a").removeClass('active-button');
			var thisIndex = $(this).index();
			$('#menu-make-main > li').each(function (eachIndex) {
				var dynamic_content;
				if ($(this).hasClass('menu-item-has-children')) {
					switch(true){
						case $(this).hasClass('Projects'):
							dynamic_content = '.latest-projects';
							break;
						case $(this).hasClass('Stories'):
							dynamic_content = '.latest-stories';
							break;
						case $(this).hasClass('Events'):
							dynamic_content = '.latest-events';
							break;
						case $(this).hasClass('Shop'):
							dynamic_content = '.latest-shop';
							break;
						case $(this).hasClass('Guides'):
							dynamic_content = '.nav-guides';
							break;
						case $(this).hasClass('Get'):
							dynamic_content = '.nav-share';
							break;
					}
					if (eachIndex < thisIndex) {
						$(dynamic_content).removeClass('moove-right').addClass('moove-left').css('visibility', 'hidden');
					} else if (eachIndex > thisIndex) {
						$(dynamic_content).removeClass('moove-left').addClass('moove-right').css('visibility', 'hidden');
					} else {
						$(dynamic_content).removeClass('moove-left moove-right').css('visibility', '');
					}
				}
			});
			$('.menu-sub-menu > .sub-menu').each(function (eachIndex) {
				if (eachIndex < thisIndex) {
					$(this).removeClass('moove-right').addClass('moove-left').css('visibility', 'hidden');
				} else if (eachIndex > thisIndex) {
					$(this).removeClass('moove-left').addClass('moove-right').css('visibility', 'hidden');
				} else {
					$(this).removeClass('moove-left moove-right').css('visibility', '');
				}
			});
			$('.menu-sub-menu > .sub-menu').addClass('nav-transition');
			if ($(this).hasClass('first-hover')) {
				$('.menu-sub-menu > .sub-menu').removeClass('nav-transition');
				$('.menu-item-has-children').removeClass('first-hover');
				$('.dynamic-header-posts').slideDown('fast');
				$('.latest-projects').show();
				$('.latest-stories').show();
				$('.latest-events').show();
				$('.latest-shop').show();
				$('.nav-guides').show();
				$('.nav-share').show();
				$('.menu-sub-menu > .sub-menu').show();
			} else {
				$('.latest-projects').addClass('nav-transition');
				$('.latest-stories').addClass('nav-transition');
				$('.latest-events').addClass('nav-transition');
				$('.latest-shop').addClass('nav-transition');
				$('.nav-guides').addClass('nav-transition');
				$('.nav-share').addClass('nav-transition');
			}
			if (!$(this).children("a").hasClass('active-button')) {
				$(this).children("a").addClass("active-button");
			} else {
				$(this).children("a").removeClass("active-button");
				$('.dynamic-header-posts').slideUp('fast');
				window.setTimeout(function () {
					$('.latest-projects').hide().removeClass('nav-transition');
					$('.latest-stories').hide().removeClass('nav-transition');
					$('.latest-events').hide().removeClass('nav-transition');
					$('.latest-shop').hide().removeClass('nav-transition');
					$('.nav-guides').hide().removeClass('nav-transition');
					$('.nav-share').hide().removeClass('nav-transition');
					$('.dynamic-header-content').children().removeClass('moove-left moove-right');
					$('.menu-sub-menu > .sub-menu').hide().removeClass('nav-transition moove-left moove-right');
				}, 0);
				$('.menu-item-has-children').addClass('first-hover');
				if (!$('.menu-item-has-children > a').hasClass('active-button')) {
				}
			}
		}
	});

	// Closees nav on mouseleave
	$('#menu-make-main, .dynamic-header-posts').on('mouseleave', function () {
		window.setTimeout(function () {
			viewWidth = $(window).width() + 17;
			if ((viewWidth >= '768') && (!$('.dynamic-header-posts').is(':hover')) && (!$('#menu-make-main').is(':hover'))) {
				$('.menu-item-has-children').children("a").removeClass("active-button");
				$('.dynamic-header-posts').slideUp('fast');
				$('.latest-projects').hide().removeClass('nav-transition');
				$('.latest-stories').hide().removeClass('nav-transition');
				$('.latest-events').hide().removeClass('nav-transition');
				$('.latest-shop').hide().removeClass('nav-transition');
				$('.nav-guides').hide().removeClass('nav-transition');
				$('.nav-share').hide().removeClass('nav-transition');
				$('.dynamic-header-content').children().removeClass('moove-left moove-right');
				$('.menu-sub-menu > .sub-menu').hide().removeClass('nav-transition moove-left moove-right');
				$('.menu-item-has-children').addClass('first-hover');
			}
		}, 100);
	});

	// Tablet
	if ($('.header').hasClass('tablet')) {
		$('body').bind("touchstart", function (e) {
			viewWidth = $(window).width() + 17;
			if (viewWidth >= '768') {
				var dynamicContainer = $(".dynamic-header-posts");
				var headerContainer = $(".navbar-nav");
				if ((dynamicContainer.has(e.target).length === 0) && (headerContainer.has(e.target).length === 0)) {
					$('.menu-item-has-children').children("a").removeClass("active-button");
					$('.dynamic-header-posts').slideUp('fast');
					window.setTimeout(function () {
						$('.latest-projects').hide().removeClass('nav-transition');
						$('.latest-stories').hide().removeClass('nav-transition');
						$('.latest-events').hide().removeClass('nav-transition');
						$('.latest-shop').hide().removeClass('nav-transition');
						$('.nav-guides').hide().removeClass('nav-transition');
						$('.nav-share').hide().removeClass('nav-transition');
						$('.dynamic-header-content').children().removeClass('moove-left moove-right');
						$('.menu-sub-menu > .sub-menu').hide().removeClass('nav-transition moove-left moove-right');
					}, 0);
					$('.menu-item-has-children').addClass('first-hover');
				}
			}
		});
	}
});})(window.jQuery);
